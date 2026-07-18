# Windows capture kit — "chi mette il touchscreen in stato ricettivo?"

## Ipotesi da testare
Il touchscreen MSHW0231 su Linux riceve una DESCREQ byte-perfect ma non risponde e resta
in reset-loop perpetuo; su Windows risponde alla prima. La transazione SPI è stata replicata
e falsificata come causa. Sospetto: **un componente Windows diverso da `hidspi.sys`**
(ACPI power method, Surface Aggregator/SAM via Surface Serial Hub, gestione power/PnP) mette
il device in stato ricettivo prima/durante l'init. I trace che abbiamo (`traces/*.csv`)
catturano SOLO SPB + GPIO-interrupt + HIDCLASS → NON possono mostrare ACPI/SAM/power.

## Cosa cattura questo kit (che i vecchi trace NON avevano)
- **ACPI** (metodi power, _PS0/_INI/_RST, i 2 AcpiEventMethod visti prima dell'init touch)
- **Kernel-Power** (transizioni Dx del device: chi porta il touch a D0 e quando)
- **Kernel-PnP** (ordine di start dei device: cosa parte prima del touch)
- **Surface/SAM** (richieste Surface Aggregator: se Windows manda un comando SAM per il touch)

Il touch si inizializza a COLD BOOT (non basta disable/enable in Gestione Dispositivi, che
fa solo un resume D2→D0). Perciò si usa un **boot trace**.

## Passi (da eseguire su Windows, prompt come Amministratore)

### 1. Trova i provider Surface/SAM esatti di questa macchina
```
00_enumerate.cmd > providers.txt
```
Mandami `providers.txt`. Cerco i GUID Surface/Aggregator/ACPI e li aggiungo al profilo se
mancano. (Il profilo include già ACPI/Power/PnP per nome.)

### 2. Avvia il boot trace, riavvia, ferma
```
wpr -boottrace -addboot touch_boot.wprp
shutdown /r /t 0
```
Dopo il riavvio e il login, aspetta ~30 secondi (così il touch ha finito l'init), poi:
```
wpr -boottrace -stopboot %SystemDrive%\touch_boot.etl
```

### 3. Esporta in testo e mandami il risultato
```
02_export.cmd
```
Produce `touch_boot.csv` (+ eventuale `touch_boot.txt`). Mandami quello.

## Cosa cercherò nel log
- Un evento SAM/Surface Serial Hub attorno al power-up del touch (TC/comando specifico).
- Quale metodo ACPI sono i 2 `AcpiEventMethod` e se un _PS0/M009/M010 precede il primo
  RESET_RSP.
- L'ordine PnP: se un device companion/SAM parte e abilita qualcosa prima del touch.
- Transizioni D0/Dx del touch e di eventuali device correlati.

## In parallelo (facoltativo, lato Linux)
Se il tuo kernel ha gli eventi ftrace `ssam_*`, possiamo loggare cosa fa SAM su Linux e
confrontarlo. Su questo boot non erano compilati; se ti interessa lo verifichiamo.

---

## AGGIORNAMENTO dopo providers.txt (07/2026)

`providers.txt` ha rivelato l'architettura reale del touch su Windows e conferma il sospetto
"un componente non-hidspi abilita il touch". Il profilo `touch_boot.wprp` ora usa i GUID VERI
di questa macchina. Cosa cercare nel log:

- **`ACPI\MSHW0231\A` = "Surface Digitizer HidSpi Extn Package"** — NON e' hidspi.sys puro:
  c'e' un **pacchetto di estensione vendor** del digitizer. E' il primo candidato a fare
  l'init speciale che a noi manca.
- Il touch e' HID multi-collezione: **COL01 "Touch Communications"** (canale di CONTROLLO,
  distinto dal touch dati), COL06 "Touch Screen Device", + pen/digitizer/VHF enum.
- **Surface Serial Hub (MSHW0084)** + **SMF Core/Client** + **SMF Display Client**: lo stack
  SAM che potrebbe alimentare/abilitare il pannello prima del touch.
- **"ACPI Driver Trace Provider"** (GUID DAB01D4D...) traccia l'esecuzione dei metodi ACPI —
  cioe' potra' finalmente dirci COSA sono i 2 `AcpiEventMethod` visti prima dell'init touch.

Il profilo cattura tutto questo + SPB/GPIO/HIDCLASS sulla stessa timeline, cosi' posso
correlare "richiesta SAM / metodo ACPI  ->  primo RESET_RSP del touch".

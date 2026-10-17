# Qualificazione hardware del touchscreen (guida rapida)

Un comando raccoglie un bundle di qualificazione **onesto e ripetibile**: preflight
(identità e hardware), snapshot in sola lettura, cattura gesti limitata e un verdetto tuo.

`tools/qualify-touch.py` è in sola lettura: non installa, non carica o scarica moduli,
non riavvia, non sospende, non tocca chiavi o MOK, non fa upload e non scala privilegi.
Se servono `/dev/input` e i log del kernel lo avvii tu: lo script non usa mai `sudo`.

## 0. Prima di iniziare

Il runner qualifica **un driver già installato**, non lo installa. Prima conserva il
checkout noto-buono, le configurazioni originali e un accesso tastiera/SSH indipendente.
Sul Surface, dal clone pulito del branch test, raccogli prima lo stato precedente e
installa il candidato con l'installer esistente (nessun comando qui è stato eseguito):

```bash
sudo bash tools/hardware_evidence/collect.sh --output "$HOME/sl4a-before-install"
./tools/sl4a-touch.sh install --standard --check
sudo ./tools/sl4a-touch.sh install --standard
```

Prima dell'installazione conserva anche una copia dei file esistenti
`/etc/modprobe.d/sl4a-spi-hid.conf` e `/etc/systemd/system/sl4a-touch-activate.service`,
annotandone l'eventuale assenza. Con Secure Boot servono la coppia DKMS **già completa**
(`mok_signing_key` + `mok_certificate`) e il suo certificato già enrolled. L'installer
può proporre creazione/importazione MOK se mancanti: **fermati**, non accettare né usare
`--rotate-mok`, `--force` o `--repair` per aggirare un rifiuto. La preparazione/enrollment
MOK resta una procedura separata consapevole. Il runner non la effettua.
L'installazione abilita il servizio e attiva subito il driver: non è un dry-run.
Poi esegui il runner sotto; albero sporco o stamp di altro commit fanno fallire i gate.

## 1. Comando unico

```bash
./tools/qualify-touch.py --phase initial \
  --output ~/sl4a-qualification/$(date -u +%Y%m%dT%H%M%SZ)-initial
```

Default: standard (un dito), `--duration 20` secondi (5..600), output **nuovo e fuori
dal repository**. Stampa i gesti, aspetta Invio, fa la cattura limitata, poi chiede **una
sola risposta** per tutto il set: `p` pass, `f` fail, `n` non osservato. Standard: tap,
linea lenta, drag veloce, drag fino a **bordi e angoli**. Bridge: anche pinch, pressa a
cinque dita con rilascio simultaneo, lift+recontact immediato, tap a due dita. Lo scroll
**non** è scontato: se una lista scorra lo decide l'app, non il pannello.

## 2. Se lo avvii con sudo (git e proprietà dell'albero)

Root serve solo per `/dev/input` e `journalctl`: meglio dare al tuo utente il gruppo
`input`. Con `sudo python3 tools/qualify-touch.py` git gira come root in un checkout tuo
e rifiuta con "dubious ownership": `repo-clean` fallisce e `preflight.txt` nomina il
problema. Mai `*` nella configurazione globale: consenti **solo quel clone esatto**:

```bash
sudo git config --global --add safe.directory /percorso/esatto/del/clone
```

## 3. Fasi: `initial`, `coldboot`, `resume`

Ripeti lo stesso comando con `--output` nuovo e `--phase` diversa. La fase è una **tua
dichiarazione**: ogni bundle registra `boot_id` e `uptime_seconds`, che chi valuta confronta.

## 4. Cosa contiene il bundle

| percorso | contenuto |
| --- | --- |
| `status.json` | esito, exit code, gate, fase, kernel, `host_root`, orari |
| `preflight.txt` | un `result=pass\|fail` per ogni gate, con il dettaglio |
| `snapshot/` | config, unità e `host.txt` (servizio, uname, `boot_id`, uptime, DKMS, Secure Boot, parametri, percorsi sysfs) più `modinfo.txt` |
| `collection/`, `collection.log` | artefatti di `tools/hardware_evidence/collect.sh` e il suo output |
| `capture/`, `capture.log` | artefatti di `capture_linux_trace_bundle.sh` e suo output |
| `observations.txt` | gesti richiesti e verdetto unico dichiarato |
| `checksums.txt` | SHA-256 di tutto il resto |

Nessun materiale MOK o chiave privata viene raccolto; integrità con
`cd <output> && sha256sum -c checksums.txt`. `status.json` e `checksums.txt` si scrivono
sempre, quindi il bundle parziale resta anche se un passo fallisce.

## 5. Gate (un fail = nessuna cattura, exit 2)

| gate | richiede |
| --- | --- |
| `repo-clean` | `git status --porcelain` con **rc 0 e output vuoto**: un git fallito non è un albero pulito |
| `build-identity` | `installed-head` **uguale** a `git rev-parse HEAD` |
| `dkms-version` | rc 0 e **una sola** riga `installed` di `sl4a-touch` per il kernel corrente; altri pacchetti, altri kernel e righe in conflitto falliscono |
| `loaded-<mod>` | directory del modulo in sysfs: `/sys/module/sl4a_spi_amd` e `sl4a_spi_hid`, **underscore** (i trattini valgono solo per `modinfo`) |
| `vermagic-<mod>` | vermagic del modulo = kernel in esecuzione |
| `srcversion-<mod>` | srcversion caricata = srcversion su disco |
| `params-standard` | `raw_mode=N` **e** `std_raw_transition=0` |
| `params-bridge`, `acpi-mshw0231` | `raw_mode=N`, `std_raw_transition=1`, `raw_input_beta=Y`, nodo touch **esattamente** `MSHW0231` |

`installed-head`, `vermagic` e `srcversion` sono **controlli di coerenza**, non
un'attestazione: dicono che stamp, modulo su disco e modulo caricato concordano, non da
quale sorgente esatto è stato compilato il codice in esecuzione.

## 6. Variante bridge (opt-in)

Solo **dopo** controllo standard riuscito e su MSHW0231, conserva la configurazione
standard. Con consenso a perdere temporaneamente il touch, usa
`sudoedit /etc/modprobe.d/sl4a-spi-hid.conf`: nella riga `options sl4a_spi_hid` imposta
`raw_mode=0 std_raw_transition=1 raw_input_beta=1`, lasciando invariati gli altri parametri.
Dalla console indipendente ricarica esplicitamente (non è fatto dal runner):

```bash
sudo modprobe -r sl4a_spi_hid sl4a_spi_amd
sudo ./tools/sl4a-touch.sh activate
sudo python3 tools/qualify-touch.py --bridge --phase initial --output "$HOME/sl4a-qualification/bridge-initial"
```

Non forzare unload se occupato o bloccato: STOP e rollback. Il runner legge i
parametri caricati davvero; modificare il solo file non basta.

Accettata solo con nodo ACPI **esattamente** `MSHW0231`: il tool cerca gli ID esatti del
driver (`MSHW0162`, `MSHW0230`, `MSHW0231`) e rifiuta sia un ripiego sia una macchina
che espone insieme più nodi touch. Servono i tre parametri bridge; il profilo standard
**non** accetta il bridge (`std_raw_transition=1` fa fallire `params-standard`). Il nodo
input lo identifica l'helper di cattura, mai "il primo dispositivo touch disponibile".

## 7. Exit code

| exit | stato | significato |
| --- | --- | --- |
| 0 | `qualified` | gate ok, cattura ok, gesti dichiarati `pass` |
| 2 | `preflight-failed` | identità, albero, versione, modulo o parametri non corrispondono |
| 3 | `capture-failed` | helper di cattura fallito o fuori tempo massimo |
| 4 | `operator-fail` | hai dichiarato `fail` |
| 5 | `incomplete` | `not observed`, oppure EOF a un prompt |
| 6 | `collection-failed` | `collect.sh` è fallito: nessuna evidenza host, nessuna cattura |
| 130 | `interrupted` | Ctrl-C/SIGTERM: gruppo di cattura terminato (TERM, pausa, poi KILL) |

## 8. Limiti da leggere

- Fase e verdetto sono **dichiarazioni tue**; il tool non deduce mai un pass.
- `SL4A_QUALIFY_HOST_ROOT` diverso da `/` significa **sandbox** (valore in `status.json`):
  quel bundle non è una qualificazione, e nessun upload parte da solo.
- Pulizia: vengono segnalati solo i processi della sessione creata dalla cattura.

## 9. Rollback

```bash
sudo systemctl disable --now sl4a-touch-activate.service   # 1. niente attivazione al boot
sudo ./tools/sl4a-touch.sh uninstall                        # 2. pacchetto, DKMS, unità
sudo reboot                                                 # 3. torna il driver in-tree
```

Tieni sempre un accesso locale o remoto e un checkout noto-buono.

## 10. Test del runner senza hardware

```bash
cd tests && make test                 # include touch_qualification_test.py
python3 touch_qualification_test.py   # solo la suite del qualificatore
```

La suite crea un repo usa-e-getta (git vero), un albero host finto sotto
`SL4A_QUALIFY_HOST_ROOT` con la struttura sysfs reale (underscore, più alberi esca con
trattini che nessun kernel crea), helper finti e stub in `PATH`. Copre successo standard
e bridge, ogni gate di identità, DKMS e git anomali, raccolta fallita, i tre verdetti,
cattura fallita, interruzione e pulizia con figlio che ignora TERM, rifiuto dei percorsi.

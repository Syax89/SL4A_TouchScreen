# Qualificazione hardware del touchscreen (guida rapida)

Un comando raccoglie un bundle di qualificazione **onesto e ripetibile**: preflight
(identità e hardware), snapshot in sola lettura, cattura gesti limitata e un verdetto tuo.

`tools/qualify-touch.py` è in sola lettura: non installa, non carica o scarica moduli,
non riavvia, non sospende, non tocca chiavi o MOK, non fa upload e non scala privilegi.
Se servono `/dev/input` e i log del kernel lo avvii tu: lo script non usa mai `sudo`.
L'unica eccezione è `--guided` (§11): dopo **un** consenso digitato ricarica il modulo a
runtime per la variante bridge, senza mai scrivere la configurazione persistente.

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
| `bound-panel`, `bound-controller` | solo `--guided`: pannello su `sl4a_spi_hid`, controller su `sl4a_spi_amd_v2_multi`; un altro driver non viene mai scalzato |

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
Il percorso **automatico** equivalente è `--guided` (§11): fa lui il cambio a runtime e
poi il ripristino, senza toccare `/etc/modprobe.d`.

Accettata solo con nodo ACPI **esattamente** `MSHW0231`: il tool cerca gli ID esatti del
driver (`MSHW0162`, `MSHW0230`, `MSHW0231`) e rifiuta sia un ripiego sia una macchina
che espone insieme più nodi touch. Servono i tre parametri bridge; il profilo standard
**non** accetta il bridge (`std_raw_transition=1` fa fallire `params-standard`). Il nodo
input lo identifica l'helper di cattura, mai "il primo dispositivo touch disponibile".

## 7. Exit code

| exit | stato | significato |
| --- | --- | --- |
| 0 | `qualified` | percorso a verdetto unico: gate ok, cattura ok, gesti dichiarati `pass` |
| 0 | `guided-qualified` | `--guided`: tutte le finestre valutate `pass`, ripristino ok |
| 2 | `preflight-failed` | identità, albero, versione, modulo o parametri non corrispondono |
| 3 | `capture-failed` | helper di cattura fallito o fuori tempo massimo |
| 3 | `transition-failed` | `--guided`: unload/load del bridge o rebind fallito (nessun parametro forzato) |
| 4 | `operator-fail` | hai dichiarato `fail` (percorso con un solo verdetto) |
| 4 | `gesture-fail` | `--guided`: almeno un gesto non è passato (motivo in `machine-results.json`) |
| 5 | `incomplete` | `not observed`, EOF a un prompt, o consenso `--guided` non dato |
| 6 | `collection-failed` | `collect.sh` è fallito: nessuna evidenza host, nessuna cattura |
| 7 | `restore-failed` | `--guided`: ripristino di parametri/binding non riuscito — `recovery.txt` e istruzioni a video |
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

## 11. Percorso automatico: `--guided` (un comando, un consenso digitato)

```bash
sudo python3 tools/qualify-touch.py --guided
```

Fa tutto da solo, con un solo prompt digitato e poi **solo tocchi sullo schermo**:

1. preflight identico a §5, più i gate di binding (`bound-panel`, `bound-controller`);
2. sceglie **il** nodo touch: solo un event node discendente dal pannello `MSHW0231` **e**
   con `INPUT_PROP_DIRECT` — mai tastiera, touchpad o altri dispositivi; se i candidati
   sono ambigui si ferma e li elenca;
3. chiede **prima dei gesti** il consenso digitato `BRIDGE`, con comandi e backup
   dei parametri/binding; qualunque altra risposta annulla senza reload;
4. tre finestre standard: tap, movimento continuo, bordi/angoli normalizzati
   su ABS min/max letti dal dispositivo; se standard fallisce non passa al bridge;
5. passa al bridge **a runtime**, senza scrivere alcun file persistente:
   `modprobe -r sl4a-spi-hid` + `modprobe sl4a-spi-hid raw_mode=0 std_raw_transition=1
   raw_input_beta=1` (gli altri parametri restano quelli letti dal modulo vivo), poi
   attende il rebind e **rilegge** i parametri caricati;
6. sei finestre bridge: tap, movimento, bordi, hold a cinque dita con rilascio simultaneo,
   pinch fuori/dentro, recontact con tracking ID **nuovo** (riuso slot registrato);
7. ripristina binding e **tutti** i parametri originali sempre: successo, eccezione,
   Ctrl-C, SIGTERM. Se il ripristino non riesce: exit 7, `recovery.txt` con i comandi e
   messaggio a video.

Ogni finestra è limitata da `--gesture-timeout` (default 12 s). Il flusso evdev viene
letto con `select`/`os.read` e valutato ai confini `SYN_REPORT`; `SYN_DROPPED`, record
parziale, EOF, timeout, contatti già attivi all'apertura e contatti ancora giù alla fine
**fanno fallire** la finestra (fail closed).

### Soglie numeriche (taratura per pannello)

```bash
sudo python3 tools/qualify-touch.py --guided \
  --knob tap_max_ms=500 --knob min_span_pct=40 --knob span_out_pct=20
```

`tap_max_ms`, `tap_move_pct`, `min_span_pct`, `min_frames`, `max_gap_ms`, `edge_pct`,
`fingers`, `release_skew_frames`, `span_out_pct`, `span_in_pct`, `recontact_ms`,
`recontact_pct`, `hold_min_ms`. I valori usati sono scritti in `machine-results.json`.

### Bundle

`raw/<profilo>-<gesto>.evdev` (byte grezzi del **solo** nodo scelto), `machine-results.json`
(metriche, soglie, provenienza del range ABS), `verdicts.txt`, `transition.txt` (comandi
eseguiti e ripristino), `secureboot.txt` (stato Secure Boot, firmatario, certificati
enrolled **contati**: nessuna chiave letta), `recovery.txt` solo se serve, `checksums.txt`.

### Verdetto onesto (PR 87–91)

`verdicts.txt` contiene voci volutamente ristrette: `standard_evdev_gestures`,
`bridge_evdev_gestures`, `pr87_bridge_standard_transport`, `persistence_after_release`; sempre
`NOT_TESTED` per `coldboot_<fase>`, `resume_<fase>` e `secureboot_enrollment`; sempre
`full_pr_pass: NOT_APPLICABLE`. Un subtest di gesti sul flusso evdev **non** è
l'accettazione di un PR, **non** è l'accettazione di un gesto da parte di desktop o
applicazione (quella è politica sopra evdev) e non dice nulla su cold boot, resume o
enrollment. PR87 riporta soltanto il subtest bridge; PR90 soltanto la raccolta
`collect.sh`. PR88 (coalescing), PR89 (identità firma) e PR91 (bounds del trasporto)
restano NOT_TESTED: i gesti non provano quei contratti. Il protocollo Type A non è
valutato; sono supportati single-touch standard e multitouch Type B.

Il ripristino è tentato e verificato, non garantito in caso di crash kernel,
SIGKILL o perdita di alimentazione: in quei casi nessun finally può eseguire.
La configurazione persistente non viene modificata.

### Cosa resta manuale

- **Cold boot e resume**: fai tu il power-on o il suspend/resume e rilancia lo stesso
  comando con `--phase coldboot|resume` e `--output` nuovo; ogni bundle porta `boot_id` e
  `uptime_seconds` da confrontare.
- **Enrollment MOK e Secure Boot**: restano una procedura separata e consapevole. Il
  percorso automatico non fa `mokutil --import`, non riavvia e non sospende: nessun
  automatismo di reboot/resume esiste qui.
- Se `modprobe -r` fallisce (modulo occupato) non si forza niente: exit 3 e nessun
  parametro cambiato. Non c'è `--force` né `rmmod -f` da nessuna parte.
- Serve `sudo` (lettura `/dev/input`, `modprobe`), ma **nessuna GUI come root**.

# Beta Multitouch Observation

## Scope

This is one controlled Linux observation after ID5-only activation. The boot
profile enabled `raw_capture_only=Y`, `raw_input_beta=Y`, and the one-shot
isolated ID5 harness. It did not send ID56 or any retry/recovery command.

The driver registered a separate input device named `MSHW0231 Touchscreen` on
`/dev/input/event6`. Its advertised capabilities included Type-B multitouch:

```text
ABS_MT_SLOT            0..46
ABS_MT_TRACKING_ID     0..65535
ABS_MT_POSITION_X/Y    0..32767
ABS_MT_TOUCH_MAJOR/MINOR
ABS_MT_ORIENTATION
```

## Observation

During the requested two-finger interaction, `evtest` observed:

- 16 positive `ABS_MT_TRACKING_ID` assignments;
- 16 corresponding `ABS_MT_TRACKING_ID=-1` releases;
- activity on slots `0`, `1`, and `2`;
- continuous Type-B coordinates, shape values, orientation, and `BTN_TOUCH`
  transitions.

This proves the complete Linux pipeline:

```text
ID5 activation -> ID0c CapImg stream -> strict decoder -> contact extractor
-> Type-B multitouch input events
```

## Limitation

The requested gesture was intended to involve two fingers, while the beta
stream used three slots. Without a labelled frame-to-contact oracle this is
not sufficient to label slot 2 as a false contact, but it is a strong reason
not to enable the beta by default. The current extractor can split one contact
or create unstable identities during a gesture.

The beta is therefore evidence of acquisition and event plumbing only. It is
not yet a usable multitouch driver.

## Recovery

The immediately following boot restored:

```text
raw_mode=N
raw_capture_only=Y
raw_input_beta=N
raw_transition_once=N
isolated_set_test=N
```

Normal MSHW0231 HID enumeration is active again. The next engineering task is
to use the recovered Surface threshold/tracker behavior and labelled captures
to replace the heuristic component/slot assignment before another beta run.

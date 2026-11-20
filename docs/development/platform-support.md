# Platform Support Policy

## Product principle

Serve the largest practical group of ordinary users first. Platform priority is determined by consumer reach, user habits, and the cost of delivering a reliable experience—not by developer preference or technical novelty.

## Phases

| Platform | Phase | Commitment |
|---|---|---|
| Windows 10/11 64-bit | 1 — First | Full initial Viewer feature development, performance optimization, usability work, and release validation |
| macOS | 2 — Second | Consumer-facing adaptation after the Windows core experience is established |
| Linux | 3 — Third | Supplementary platform support; avoid delaying the core product |
| Android | Later | Lightweight mobile client after the PC product is mature |

## Initial Viewer engineering allocation

| Platform | Planning allocation |
|---|---:|
| Windows | 75% |
| macOS | 15% |
| Linux | 10% |

These percentages are planning assumptions, not market-share statistics. Android is not included in the initial PC Viewer allocation.

## CI policy

- Windows CI is the primary product build and test gate.
- Ubuntu CI is retained as a low-cost portability smoke test; it does not imply equal Linux product priority.
- macOS CI and full platform validation are introduced in Phase 2.
- Android has a separate future client plan and is not part of the PC Viewer build matrix today.

## Minimum-hardware product goals

The initial Windows experience should aim to:
- Run basic world access, social interaction, and building on integrated graphics.
- Remain usable with 8 GB system memory.
- Allow users to lower resolution, view distance, shadows, and anti-aliasing.
- Avoid requiring a discrete GPU for basic use.
- Avoid cloud GPU rendering as a product dependency.
- Offer scalable graphics profiles rather than imposing one visual-quality target on every PC.

These are product goals to validate through profiling and real hardware testing, not claims that the current prototype already meets them.

### Recent changes

Focused correctness pass on the `haishoku` and `wal` backends to bring output in line with pywal:

- **Haishoku backend is now end-to-end native.** Replaced the external Python helper (`/home/sleek/.local/bin/extract_colors` + `/tmp/colors.txt`) with direct pixel iteration through `Magick::Pixels`, clamping HDRI overshoot to 0–255 and flattening alpha/profile before counting. CURL dependency and remote-URL support removed.
- **Haishoku algorithm bugs fixed.** `HaishokuAlgo::groupByAccuracy` was bucketing green/blue against the red threshold; `rgbMaximum` was reading the red index for every channel. `Haishoku::getColorsMean` now sorts by pixel count (not RGB) and keeps the top-8 most frequent groups (not the bottom 8). `Haishoku::get` rewritten to mirror `pywal/pywal/backends/haishoku.py`, with safe padding for low-variance images.
- **Color primitives match pywal.** `Color::lighten` / `Color::darken` now use pywal's per-channel formulas instead of Qt's HSV-based `QColor::lighter()`/`darker()`. `Color::c_saturate` rewritten against HSL (`hslHueF` + `fromHslF`) to match `util.saturate_color`. `Color::genericAdjust` light branch reshaped so it has the same observable behaviour as pywal's dead-loop.
- **Wal backend correctness follows from the primitive fix** — no changes to `Wal::adjust`, but its output now lines up with pywal because `lighten`/`darken` are correct.
- **`--alpha` flag now persists.** `Color::alphaValue` was a per-instance field that defaulted to 100; the CLI value was set on a throwaway object and never reached the JSON writer. Made it a static class member (matches pywal's `util.Color.alpha_num`).
- **Saturation default bug.** The `saturate` CLI flag defaulted to `"0"`, which `getColors` treated as "apply saturation at level 0" — desaturating every colour to grey. Default is now an empty string.
- **Build flags synced to the installed ImageMagick.** `CMakeLists.txt` now pulls `Magick++` cflags/libs via `pkg-config`, so `MAGICKCORE_HDRI_ENABLE` and `MAGICKCORE_QUANTUM_DEPTH` match the library the system actually links against. Previously the mismatch produced negative channel values and corrupt palettes.
- **Dead code removed.** `Util::genericAdjust`, `Util::saturateColors`, `Util::extractMaxColoursPillow`, and the orphan `hexToRgb` / `rgbToHex` stubs.

#### Verified against pywal on the haishoku demo image

| Backend  | Mode  | Derived colours (bg/fg/cursor/0/7/8/15) | Palette colours (1-6) |
|----------|-------|------------------------------------------|-----------------------|
| haishoku | dark  | exact match                              | ≤ 6/255 per channel   |
| haishoku | light | exact or ±1/255                          | ≤ 6/255 per channel   |
| wal      | dark  | bg/color0 ±20/255 (ImageMagick quantiser non-determinism) | colours 1-6 exact match |

Cache round-trips are byte-stable; solid-colour images no longer crash.
# Signal-Processing-Labs

C++17 implementation of spectral analysis for an exponentially decaying signal `x(t) = A·e^(−αt)`. Computes both the continuous Fourier spectrum and its discrete approximation (DFT), then evaluates the approximation error via RMSE.

## Tech Stack

C++17, g++, `std::complex<double>`

## How It Works

The signal analyzed is a one-sided exponential decay. Two spectra are computed and compared:

**Continuous spectrum** — closed-form analytical solution via Fourier transform:
```
X(ω) = A / (α + jω)
```

**Discrete spectrum** — numerical DFT computed directly from samples:
```
X(k) = T · Σ A·e^(−αnT) · e^(−j2πkn/N)
```

The window length is hardcoded as `tu = 0.4s`, so `N = 0.4 / T` samples are used regardless of `num_points`. RMSE between the two spectra is printed at the end.

## Notable Details

- The sampling window `tu = 0.4s` is baked into `DiscreteSpectrum()` as a magic constant — `N` is derived from it, not from `num_points`. This means `num_points` only controls how many frequency bins are printed, not the DFT length.
- `std::complex<double>` arithmetic is used throughout — `A / (alpha + j·ω)` is computed directly via C++ complex division without manually separating real/imaginary parts.
- RMSE is computed as root mean square of `|X_discrete(k) − X_continuous(ω_k)|` — a point-wise comparison in the complex magnitude sense.

## Quick Start

```bash
cd src
make build
```

Input when prompted:
```
A      — signal amplitude
alpha  — decay rate
T      — sampling period (seconds)
N      — number of spectrum points to display
```

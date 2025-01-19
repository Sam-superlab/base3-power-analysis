# Base-3 Power of 2 Analysis

This project analyzes the distribution of digits in the base-3 representation of powers of 2. It efficiently computes 2^a in base 3 for large values of a (up to 100,000 or more) and analyzes the frequency distribution of digits (0, 1, 2).

## Features

- Fast computation of base-3 representation of 2^a using incremental multiplication
- Tracks frequency of digits (0, 1, 2) in the representations
- Memory efficient implementation using uint8_t arrays
- Generates detailed CSV data for analysis
- Creates beautiful visualizations of digit distributions and trends

## Requirements

### C Compiler
- GCC or compatible C compiler
- Make

### Python (for visualization)
- Python 3.6+
- Required packages in `requirements.txt`:
  - pandas
  - matplotlib
  - seaborn

## Installation

1. Clone the repository:
```bash
git clone https://github.com/Sam-superlab/base3-power-analysis.git
cd base3-power-analysis
```

2. Build the C program:
```bash
make
```

3. Install Python dependencies:
```bash
pip install -r requirements.txt
```

## Usage

1. Run the analysis program:
```bash
./base3power [exponent]
```
Where `exponent` is the maximum power of 2 to compute (default: 100000).

2. Generate visualizations:
```bash
python3 plot_frequencies.py
```

This will create `digit_analysis.png` with three plots:
1. Absolute digit counts (log scale)
2. Percentage distribution with moving averages
3. Deviation from theoretical equal distribution

## Output Files

- `frequency_data.csv`: Raw data containing counts and percentages for each power
- `digit_analysis.png`: Visualization of the analysis

## Example Results

For powers of 2 up to 10,000, the distribution of digits approaches:
- Zeros: ~33.33%
- Ones: ~33.33%
- Twos: ~33.33%

This suggests that the digits in the base-3 representation of large powers of 2 tend toward an equal distribution.

## Implementation Details

The program uses an incremental approach:
1. Starts with 2^1 = 2 in base 3
2. Repeatedly multiplies by 2 in base 3 to get successive powers
3. Counts and accumulates digit frequencies

- Time Complexity: O(A²) where A is the maximum exponent
- Space Complexity: O(A) for storing digits and partial sums

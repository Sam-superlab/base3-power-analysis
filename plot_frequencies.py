import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

# Set style
plt.style.use('seaborn-v0_8')
sns.set_palette("husl")

# Read the CSV data
df = pd.read_csv('frequency_data.csv')

# Calculate moving averages (window of 100 powers)
window = 100
df['zeros_ma'] = df['zeros_percent'].rolling(window=window).mean()
df['ones_ma'] = df['ones_percent'].rolling(window=window).mean()
df['twos_ma'] = df['twos_percent'].rolling(window=window).mean()

# Create figure and subplots
fig = plt.figure(figsize=(15, 12))
gs = fig.add_gridspec(3, 1, height_ratios=[2, 2, 1])
ax1 = fig.add_subplot(gs[0])
ax2 = fig.add_subplot(gs[1])
ax3 = fig.add_subplot(gs[2])

fig.suptitle('Analysis of Base-3 Digits in Powers of 2', fontsize=14, y=0.95)

# Plot 1: Absolute counts with log scale
ax1.plot(df['power'], df['zeros'], label='Zeros', linewidth=1, alpha=0.7)
ax1.plot(df['power'], df['ones'], label='Ones', linewidth=1, alpha=0.7)
ax1.plot(df['power'], df['twos'], label='Twos', linewidth=1, alpha=0.7)
ax1.set_xlabel('Power of 2')
ax1.set_ylabel('Number of Digits (log scale)')
ax1.set_title('Absolute Digit Counts')
ax1.legend()
ax1.grid(True)
ax1.set_yscale('log')

# Plot 2: Percentages with moving average
ax2.plot(df['power'], df['zeros_percent'],
         label='Zeros %', linewidth=1, alpha=0.3)
ax2.plot(df['power'], df['ones_percent'],
         label='Ones %', linewidth=1, alpha=0.3)
ax2.plot(df['power'], df['twos_percent'],
         label='Twos %', linewidth=1, alpha=0.3)
ax2.plot(df['power'], df['zeros_ma'],
         label=f'Zeros ({window}-power MA)', linewidth=2)
ax2.plot(df['power'], df['ones_ma'],
         label=f'Ones ({window}-power MA)', linewidth=2)
ax2.plot(df['power'], df['twos_ma'],
         label=f'Twos ({window}-power MA)', linewidth=2)
ax2.set_xlabel('Power of 2')
ax2.set_ylabel('Percentage')
ax2.set_title('Digit Distribution Percentages')
ax2.legend()
ax2.grid(True)

# Add theoretical distribution line
ax2.axhline(y=100/3, color='gray', linestyle='--', alpha=0.5,
            label='Theoretical Equal Distribution (33.33%)')

# Plot 3: Deviation from theoretical equal distribution
theoretical = 100/3
ax3.plot(df['power'], df['zeros_percent'] - theoretical,
         label='Zeros deviation', linewidth=1)
ax3.plot(df['power'], df['ones_percent'] - theoretical,
         label='Ones deviation', linewidth=1)
ax3.plot(df['power'], df['twos_percent'] - theoretical,
         label='Twos deviation', linewidth=1)
ax3.set_xlabel('Power of 2')
ax3.set_ylabel('Deviation from 33.33%')
ax3.set_title('Deviation from Equal Distribution')
ax3.legend()
ax3.grid(True)
ax3.axhline(y=0, color='black', linestyle='-', alpha=0.3)

# Add text with statistics
final_stats = f"""Final Statistics (n={len(df):,}):
Zeros: {df['zeros_percent'].iloc[-1]:.2f}%
Ones:  {df['ones_percent'].iloc[-1]:.2f}%
Twos:  {df['twos_percent'].iloc[-1]:.2f}%"""
fig.text(0.02, 0.02, final_stats, fontsize=10, family='monospace')

# Adjust layout and save
plt.tight_layout()
plt.savefig('digit_analysis.png', dpi=300, bbox_inches='tight')
plt.close()

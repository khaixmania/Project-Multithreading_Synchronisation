import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("analyses/producer_consumer.csv")
grouped = data.groupby("thread")["time"].agg(["mean","std"]).reset_index()
plt.figure(figsize=(8,6))
plt.errorbar(grouped['thread'], grouped['mean'], yerr=grouped['std'], fmt='o-', capsize=5)
plt.xticks(grouped['thread'])

plt.xlabel("Nombre de threads")
plt.ylabel("Temps d'exécution (secondes)")
plt.title("Temps d'exécution vs Nombre de threads")
plt.ylim(bottom=0)
plt.grid(True)
plt.savefig('pc_plot.pdf', dpi=300)

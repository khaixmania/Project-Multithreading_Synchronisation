#Tâche 2.2 — graphes
import pandas as pd
import matplotlib.pyplot as plt

data1 = pd.read_csv("analyses2/test1.csv")
grouped1 = data1.groupby("thread")["time"].agg(["mean","std"]).reset_index()
data2 = pd.read_csv("analyses2/test2.csv")
grouped2 = data2.groupby("thread")["time"].agg(["mean","std"]).reset_index()

plt.figure(figsize=(8,6))

plt.errorbar(grouped1['thread'], grouped1['mean'], yerr=grouped1['std'], fmt='o-', capsize=5, label='Test-and-Set')
plt.errorbar(grouped2['thread'], grouped2['mean'], yerr=grouped2['std'], fmt='o-', capsize=5, label='Test-and-Test-and-Set')

plt.xticks(grouped1['thread'])
plt.xlabel("Nombre de threads")
plt.ylabel("Temps d'exécution (secondes)")
plt.title("Temps d'exécution vs Nombre de threads")
plt.ylim(bottom=0)
plt.legend()
plt.grid(True)
plt.savefig('tests.pdf', dpi=300)

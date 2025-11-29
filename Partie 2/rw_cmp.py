import pandas as pd
import matplotlib.pyplot as plt

data1 = pd.read_csv("../Partie 1/analyses/reader_writer.csv")
grouped1 = data1.groupby("thread")["time"].agg(["mean","std"]).reset_index()

data2 = pd.read_csv("analyses2/reader_writer_spin.csv")
grouped2 = data2.groupby("thread")["time"].agg(["mean","std"]).reset_index()

plt.figure(figsize=(8,6))

plt.errorbar(grouped1['thread'], grouped1['mean'], yerr=grouped1['std'], fmt='o-', capsize=5, label='POSIX (RWLock)')
plt.errorbar(grouped2['thread'], grouped2['mean'], yerr=grouped2['std'], fmt='o-', capsize=5, label='Spinlocks (test-and-test-and-set)')

plt.xticks(grouped1['thread'])
plt.xlabel("Nombre de threads")
plt.ylabel("Temps d'exécution (secondes)")
plt.title("Comparaison : Lecteurs-Ecrivains")
plt.ylim(bottom=0)
plt.legend()
plt.grid(True)
plt.savefig('rw_cmp.pdf', dpi=300)

# Project - Multithreading & Synchronisation

Ce projet explore les primitives de synchronisation POSIX (mutex, sémaphores) et l'implémentation de verrous à attente active (spinlocks) en C.

## Environnement de Test (Spécifications VM)

Les mesures de performances et les graphiques présentés dans ce projet ont été réalisés sur une machine virtuelle Ubuntu (via Multipass) ayant la configuration suivante :

| Composant | Spécification | Détails |
| :--- | :--- | :--- |
| **Système d'exploitation** | **Ubuntu 24.04.1 LTS** | Version stable Long-Term Support |
| **Processeur (vCPU)** | **4 Cœurs** | Facteur limitant observé dans les graphes de performance au-delà de 4 threads |
| **Mémoire Vive (RAM)** | **4 GB** | |
| **Espace Disque** | **20 GB** | |
| **Type de VM** | Multipass | |

> **Note sur les résultats :** La limitation matérielle à **4 vCPUs** explique pourquoi le temps d'exécution stagne ou augmente lorsque le nombre de threads dépasse 4 (saturation du parallélisme physique et augmentation de l'overhead de gestion des threads).

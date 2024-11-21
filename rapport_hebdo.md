# Rapports Hebdomadaires

## Semaine du [ mer 6 nov 2024 ] au [ mer 13 nov 2024 ]

### Objectifs de la Semaine 1

- Boucle principale avec un prompt basique dans `main.c`.
- Définir une structure pour représenter une commande et implémenter les fonctions associées.
- Implémenter une commande interne au choix

---

### Répartition et suivi de l'Avancement

Chaque membre est invité à remplir l'état d'avancement de ses tâches en fin de semaine :

- **[ Andréa ]** :

  - **Tâche** : Définir une structure pour représenter une commande
  - **Avancement** : [Description de l'avancement ici]

- **[ Sylvain ]** :

  - **Tâche** : Boucle principale, Prompt et fonctions associées
  - **Avancement** :
    - Mise en place de la boucle principale et de la bibliothèque readline pour gérer les entrées utilisateur.
    - Implémentation dans `prompt.c`du prompt complet avec le changement de couleur en fonction de la valeur de retour de la dernière commande et la troncature du prompt pour ne pas dépasser 30 caractères.

- **[ Nic ]** :
  - **Tâche** : Issue #4
  - **Avancement** : `pwd.c` et son implémentation dans le main

---

**Remarques générales :**

- **_Sylvain_**: Concernant les bascules de couleur pour le prompt, je n'ai pas ajouté les balises `\001 et \002` pour les couleurs car même sans les utiliser, le prompt est bien tronqué et change de couleur. Si c'est obligatoire je trouverais un moyen de les intégrer.

---

## Semaine du [ mer 13 nov 2024 ] au [ mer 20 nov 2024 ]

### Objectifs et répartition de la Semaine 2

- **[ Andréa ]** :

  - **Tâche** :
    - boucles `for` sans option, avec corps de boucle limité à une commande simple.
    - Implémentation des fonctions nécéssaires pour la struct command.

- **[ Nic ]** :

  - **Tâche** : Redirection vers des fichiers.

- **[ Sylvain ]** :
  - **Tâche** : Mise en place des commandes externes.
  - **Avancement:**
    - Implémentation des commandes externes dans `external_cmds.c`, grâce à la fonction `fork`qui se charge de créer un processus enfant qui va exécuter la commande en utilisant la fonction `execvp`. Les paramètres de la fonction sont obtenus à partir de la fonction `split_line`qui découpe la ligne de commande en arguments et les stockes dans un tableau de chaînes de caractères.
    - Ajout de la fonction `execute_commande`qui permet de centraliser l'exécution des commandes internes et externes.

---

**Remarques générales :**

- **_Sylvain_**: Lorsqu'on exécute une ou plusieurs commandes inexistantes et qu'ensuite on exécute `exit`, le shell ne se ferme pas. Il faut exécuter la commande plusieurs fois pour qu'elle se ferme. Le problème est probablement dû à la refactorisation du code.

---

## Semaine du [ mer 20 nov 2024 ] au [ mer 27 nov 2024 ]

### Objectifs et répartition de la Semaine 3

- **[ Andréa ]** :

  - **Tâche** : Finir les boucles `for` et les fonctions nécéssaires pour la struct command.

- **[ Nic ]** :

  - **Tâche** : Mise en place des redirections vers des fichiers.

- **[ Sylvain ]** :
  - **Tâche** : Mise en place des commandes structurées du type `cmd_1 ; cmd_2 ; ... ; cmd_n`.

---

**Remarques générales :**

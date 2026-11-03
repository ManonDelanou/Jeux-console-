# Environnement de développement C — GCC 16.2 / C11

Dossier prêt à l'emploi pour **VS Code Dev Containers**.

## Démarrage

1. Prérequis : Docker Desktop + extension VS Code **Dev Containers** (`ms-vscode-remote.remote-containers`).
2. Ouvrir **ce dossier `_env`** dans VS Code (et pas le dossier parent).
3. `F1` → **Dev Containers: Reopen in Container**.

Le dossier local est monté en **bind mount** sur `/workspaces/c-lang` : les fichiers
restent sur la machine hôte et sont éditables indifféremment depuis l'hôte ou le
conteneur. Rien n'est copié, rien n'est stocké dans un volume Docker.

## Arborescence

```
_env/
├── .devcontainer/
│   ├── Dockerfile          image gcc:16.2.0-trixie + gdb + valgrind
│   └── devcontainer.json   bind mount, extensions, ptrace pour gdb
├── .vscode/
│   ├── c_cpp_properties.json   IntelliSense C11
│   ├── launch.json             debug du fichier ouvert
│   └── tasks.json              build / run / memcheck / clean
├── include/                en-têtes (.h) partagés
├── lib/                    code partagé, SANS main (lié à chaque programme)
├── src/                    un fichier = un programme (main.c, exo-01.c, …)
└── Makefile
```

## Un fichier = un programme

Chaque `src/<nom>.c` est compilé et lié **séparément**, vers `bin/<mode>/<nom>` :

| Fichier | Exécutable |
|---|---|
| `src/main.c` | `bin/debug/main` |
| `src/exo-01.c` | `bin/debug/exo-01` |
| `src/exo-06.c` | `bin/debug/exo-06` |

Chacun a donc son propre `main()`, ce qui est exactement le découpage « un
fichier par exercice ». Aucune ligne du `Makefile` à toucher : les sources sont
découvertes par `wildcard`.

Pour du code à **partager** entre plusieurs exercices, mettre les fonctions dans
`lib/*.c` (pas de `main()` là-dedans) et les prototypes dans `include/*.h`. Tout
`lib/*.c` est lié dans chaque programme.

## Compiler / exécuter / déboguer — au clavier

Aucune commande à taper. Les trois raccourcis agissent sur le **fichier ouvert
dans l'éditeur** (via `${fileBasenameNoExtension}`) :

| Raccourci | Action | Ce que ça fait |
|---|---|---|
| `Ctrl/Cmd+Shift+B` | Compiler | tâche **build** → `bin/debug/<fichier ouvert>` |
| `Ctrl+F5` | Exécuter | compile si besoin, puis lance dans le terminal |
| `F5` | Déboguer | compile, puis lance sous **gdb** aux points d'arrêt |

Ouvrir `src/exo-01.c` et faire `Ctrl+F5` lance donc `exo-01`, sans rien
configurer. Corollaire : il faut que le fichier **actif** soit le `.c` voulu —
si l'éditeur affiche un `.h` ou le `Makefile`, la cible n'existera pas.

Les deux dernières utilisent la configuration **Debug (gdb)** de `launch.json`,
avec `"console": "integratedTerminal"` : les `printf` s'affichent dans le
terminal VS Code et les `scanf` peuvent y lire la saisie.

La configuration **Debug + sanitizers (gdb)** fait la même chose sur le binaire
instrumenté ASan/UBSan — utile pour les chapitres pointeurs / mémoire.

Autres tâches via `Terminal > Exécuter la tâche...` : *build all*,
*build (sanitizers)*, *memcheck (valgrind)*, *list*, *clean*.

## Et sous le capot : le Makefile

Les tâches VS Code ne font qu'appeler `make`. Utile pour le formateur :

```bash
make                      # compile TOUS les programmes de src/
make bin/debug/exo-01     # compile seulement celui-là
make run FILE=exo-01      # compile puis exécute
make run FILE=exo-01 ARGS="a b"
make debug FILE=exo-01    # sous gdb, en ligne de commande
make memcheck FILE=exo-01 # sous valgrind
make list                 # liste les programmes disponibles
make MODE=release         # build optimisé   -> bin/release/
make MODE=san             # ASan + UBSan     -> bin/san/
make clean                # supprime build/
make fclean               # supprime build/ et bin/
make re
make help
```

`FILE` accepte `exo-01`, `exo-01.c` ou `src/exo-01.c`. Sans `FILE`, c'est
`main`. Un `FILE` inexistant est signalé par un message explicite avant toute
compilation.

## Règles de compilation

- `-std=c11` strict (pas `gnu11`) : aucune extension GNU.
- **VLA interdits** : `-Wvla -Werror=vla -Wvla-parameter`. Un tableau à taille
  variable déclenche une **erreur** de compilation, pas un simple avertissement.
- Avertissements : `-Wall -Wextra -Wpedantic -Wshadow`.
- `-lm` est lié par défaut (`sqrt`, `pow`… sans rien configurer).

### Pourquoi pas plus d'avertissements ?

`-Wstrict-prototypes`, `-Wmissing-prototypes` et `-Wwrite-strings` ont été
retirés : ils produisent du bruit sur le code du cours lui-même (`int main()`
sans `void`, fonctions définies sans prototype préalable, littéraux passés à un
`char *`). À réactiver dans `WARNINGS` si le niveau du groupe le permet.
# Jeux-console-

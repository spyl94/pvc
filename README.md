Un voyageur doit visiter n villes. Celui-ci souhaite faire une tournée en visitant chaque ville au moins exactement 1 fois et en terminant à sa ville de départ. Quel est le parcours le plus court, les distances entre chacune des villes étant connues?

#### Détails des différentes étapes algorithmiques

### Génération d'une population d'individu pseudo aléatoire.

Il s'agit ici, de générer une suite d'entiers sans doublons (chaque ville ne peut être présent qu'une seule fois dans un individu). Plutôt que de générer des nombres pseudo-aléatoires et de les ajouter à l'individu si ils n'étaient pas déjà présent nous avons choisi d'utiliser un algorithme plus efficace.
Cet algorithme reprend le principe de la distribution d'un jeu de cartes, à chaque tour le jeu est mélangé différemment donc la distribution des cartes n'est pas la même. Ici, notre jeu de carte correspond à l'ensemble des villes et chaque tour à un individu, ainsi chaque individu va posséder toutes les villes mais dans un ordre différent.
Pour cela on commence par stocker toutes les villes sauf la ville de départ dans un tableau (peu importe l'ordre). Puis pour chacun des individus on commence par ajouter la ville de départ puis l'on modifie l'ordre des villes (phase de mélange) et on les ajoutent à notre individu.

### La sélection des individus.

Le principe de la sélection est de diviser notre population par deux et d'envoyer les individus survivants à la méthode de croisement. Nous allons étudier deux méthodes différentes de sélection:
*   La méthode élitiste qui est déterministe, il s'agit de sélectionner les  meilleur moitié d'individus. Pour cela il faut calculer la distance de chacun des individus puis les classer et enfin envoyer la meilleur moitié des individus de ce classement à notre méthode de croisement.
*   La méthode de sélection par tournois qui est non déterministe, il s'agit ici d'effectuer des duels entre des couples d'individus choisis aléatoirement. Pour cela on choisit deux individus aléatoirement, on calcule leurs distances respectives et on ajoute aux survivants l'individus ayant la distance la plus faible.

### Le croisement des individus.

Cette méthode est certainement la plus importante de l'algorithme: c'est elle qui permet d'améliorer nos individus afin d'obtenir des solutions correctes à notre problème en concentrant les parties intéressantes des individus.
Notre méthode va associer à chacun des individus reçu un autre survivant afin de former un couple, puis chacun de ces couples va recevoir aléatoirement un intervalle de mutation caractérisé par un début et une largeur. Il nous faut ensuite  injecter la partie du premier parent comprise dans l'intervalle du mutation dans le deuxième enfant, et inversement pour le second parent.
L'étape la plus fastidieuse est ensuite de supprimer les villes en doubles en respectant l'ordre du parent (afin de conserver un maximum de parties intéressantes). Voici l'algorithme que nous avons utilisé:
Si dans l'individu fils une ville est la même que dans l'intervalle de croisement alors on retient sa position dans un tableau. Si l'on a trouvé des doubles alors on regarde une à une les valeurs de l'intervalle de mutation du parent et si elles sont absentes on les substituent dans l'enfant aux positions enregistrées, dans l'ordre.
Il nous reste enfin à remplacer la population précédente par la nouvelle population comprenant tous les enfants obtenus.

### L'opérateur de mutations
Cette méthode est certainement la plus simple: elle choisit un individu et inverse deux de ces villes au hasard. On envoi ainsi à cette méthode le nombre de mutations à effectuer qui correspond au taux de mutation * le nombre d'individus au sein de la population.


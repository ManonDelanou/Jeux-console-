#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// --==={  STRUCTURE  }===--

typedef struct
{
    char nom[20];
    int stats;
    int gold;
    bool statut;
} Achat;

// --==={  BOUTIQUE  }===--

void boutique(int *joueur_degat,int *joueur_defense_p,int *joueur_gold,Achat *achat_armes,Achat *achat_armures,bool *jeu){
    int choix;
    int tmp_article;
    Achat *objet = NULL;

    // --==={  TEXTE NARRATIF  }===--

    printf("============================= \n");
    printf("==Bienvenue à la boutique !== \n");
    printf("============================= \n");
    printf("Votre solde est de %d gold \n", *joueur_gold);

    // --==={  BOUTIQUE D'ARMES  }===--

    printf("Voici nos armes : \n");
    for (int i = 0; i < 4; i++)
    {
        tmp_article +=1;
        if(achat_armes[i].statut){
            printf("%d - %s : Acheté !\n", tmp_article, achat_armes[i].nom);
        }
        else{
            printf("%d - %s (+%d stats) : %d gold\n", i + 1, achat_armes[i].nom, achat_armes[i].stats, achat_armes[i].gold);
        }
    }

    // --==={  BOUTIQUE D'ARMURE  }===--

    printf("Voici nos armures : \n");
    for (int i = 0 ; i < 4; i++)
    {
        if(achat_armes[i].statut){
            printf("%d - %s : Acheté !\n", tmp_article, achat_armures[i].nom);
        }
        else{
            printf("%d - %s (+%d stats) : %d gold\n", i + 1, achat_armures[i].nom, achat_armures[i].stats, achat_armures[i].gold);
        }
    }

    printf("%d - Continnuer les achats\n",tmp_article+1);
    printf("%d - Fermer la boutique \n",tmp_article+2);
    printf("%d - Quitter la partie \n",tmp_article+3);

    // --==={  ACHAT D'ARMES  }===--
    scanf("%d", &choix);
    if (choix >= 1 && choix <= 4){
        objet = &achat_armes[choix - 1];
        if (objet != NULL)
        {
            if (objet->statut){
                printf("Deja achete !\n");
            }
            else if (*joueur_gold >= objet->gold)
            {
                printf("Vous avez acheté : %s \n-%d gold \n", objet->nom, objet->gold);
                *joueur_degat += objet->stats;
                *joueur_gold -= objet->gold;
                objet->statut = true ;
            }
            else {
                printf("Mauvais Solde !\n");
            }
        }
    }

    // --==={  ACHAT D'ARMURE  }===--

    else if (choix >= 5 && choix <=8){
        objet = &achat_armures[0];
        if (objet != NULL)
        {
            if (objet->statut){
                printf("Deja achete !\n");
            }
            else if (*joueur_gold >= objet->gold)
            {
                printf("Vous avez acheté : %s \n-%d gold \n", objet->nom, objet->gold);
                *joueur_defense_p += objet->stats;
                *joueur_gold -= objet->gold;
                objet->statut = true ;
            }
            else {
                printf("Mauvais Solde !\n");
            }
        }
    }


    // --==={  REDIRECTION BOUTIQUE  }===--

    else if (choix == tmp_article+1){
        boutique(joueur_degat, joueur_defense_p, joueur_gold, achat_armes, achat_armures, jeu);
    }
    else if (choix == tmp_article+2){
        printf("Bon courage ! \n");
    }
    else if (choix == tmp_article+3){
        printf("Aurevoir ! \n");
        *jeu = false;
    }
    else {
        printf("Choix invalide !\n");
    }
}

// --==={  POTION CHANCE  }===--

int potion(){
    int regeneration = 0;
    float potion_chance = (float)rand() / (float)RAND_MAX;
    if (potion_chance >= 0.80)
    {
        printf("Regeneration +30 ! \n");
        regeneration = 30 ;
    }else if (potion_chance >= 0.60)
    {
        printf("Regeneration +20 ! \n");
        regeneration = 20;
    }else if (potion_chance >= 0.50)
    {
        regeneration = 15;
        printf("Regeneration +15 ! \n");
    }else{
        regeneration = 0;
        printf("Potion Raté \n");
    }

    return regeneration;
}

// --==={  SELECTION DU MONSTRE  }===--

void selection_mob(char *nom,int *vie,int *degat,float *critique,int *gold,int *etage){
    int nombre = rand() % 3 + 1;
    int tmp = *etage;
    if (nombre == 1)
    {
        //Slime
        strcpy(nom, "Slime");
        *vie = 30*tmp;
        *degat= 2*tmp ;
        *critique = 0.80;
        *gold = 5*tmp;
    }else if (nombre == 2)
    {
        //Goblin
        strcpy(nom, "Goblin");
        *vie = 60*tmp;
        *degat= 5*tmp ;
        *critique = 0.60;
        *gold = 7*tmp;
    }else if (nombre == 3)
    {
        // Sorcier
        strcpy(nom, "Sorcier");
        *vie = 25*tmp;
        *degat= 8*tmp ;
        *critique = 0.20;
        *gold = 10*tmp;
    }else {
        printf("Erreur du monstre !\n");
    }

}

// --==={  SELECTION DU BOSS  }===--

void selection_boss(char *nom,int *vie,int *degat,float *critique,int *gold,int *etage){
    int nombre =  rand() % 2 + 1;
    int tmp = *etage;
    if (nombre == 1)
    {
        //slime
        strcpy(nom, "Golem");
        *vie = 500 *tmp;
        *degat= 10*tmp;
        *critique = 0.20;
        *gold = 150;
    }else if (nombre == 2)
    {
        //Goblin
        strcpy(nom, "Phantom");
        *vie = 250*tmp;
        *degat= 20*tmp;
        *critique = 0.80;
        *gold = 250;
    }else{
        printf("Erreur du monstre !\n");
    }

}

// --==={  CLASSE DU JOUEUR  }===--

void classe_joueur(int *joueur_vie,int *joueur_vie_max,int *joueur_degat, float *joueur_critique,int *joueur_defense_p,char *classe,bool *jeu){
    if (strcmp(classe, "mage") == 0 || strcmp(classe, "Mage")== 0 )
    {
        *joueur_vie = 70;
        *joueur_degat= 8 ;
        *joueur_critique = 0.80;
        *joueur_defense_p = 0;
    }else if (strcmp(classe, "guerrier") == 0 || strcmp(classe, "Guerrier") == 0)
    {
        *joueur_vie = 100;
        *joueur_degat= 3 ;
        *joueur_critique = 0.70;
        *joueur_defense_p = 20;
    }else if (strcmp(classe, "assassin") == 0 || strcmp(classe, "Assassin") == 0)
    {
        *joueur_vie = 70;
        *joueur_degat= 5 ;
        *joueur_critique = 0.60;
        *joueur_defense_p = 10;
    }else {
        printf("Erreur de classe joueur n°1 ! \n");
        *jeu = false;
    }
    *joueur_vie_max = *joueur_vie;
}

// --==={  PIEGE  }===--
//Pas encore utilise pour l'instant

int piege(){
    int tmp;
    tmp = rand() % 3 + 1;
    if (tmp == 3){
        return 30;
    }
    else if (tmp == 2){
        return 15;
    }
    else {
        return 5;
    }
}

// --==={  SALLE  }===--

void salle(char *salle_type){
    // Variable temporaire
    int r = rand() % 100;   // 0 à 99
    if (r < 30) strcpy(salle_type, "Vide");
    else if (r < 90) strcpy(salle_type, "Monstre");
    else strcpy(salle_type, "Coffre");

}

// --==={  COMBAT  }===--

void tour(int *actionneur_vie,int *joueur_vie_max,float *actionneur_critique,int *actionneur_degat,int *actionneur_defense_p,bool *actionneur_defense,int *receveur_vie,bool *receveur_defense,char *action){

        // --==={  POTION  }===--

        int vie_max = *joueur_vie_max;
        if (strcmp(action, "Potion") == 0 || strcmp(action, "potion") == 0)
        {
            *actionneur_vie += potion();
            if (*actionneur_vie > vie_max){
                *actionneur_vie = vie_max;
            }

        // --==={  DEFENDRE  }===--

        }else if (strcmp(action, "Defendre") == 0 || strcmp(action, "defendre") == 0)
        {
            printf("%d",*actionneur_defense_p);
            if ((rand() % 100) <= *actionneur_defense_p)
            {
                printf("Defense pour 1 tour !\n");
                *actionneur_defense = true;
            }
            else
            {
                printf("Defense ratee !\n");
                *actionneur_defense = false;
            }
        }

        // --==={  ATTAQUE  }===--

        else if (strcmp(action, "Attaque") == 0 || strcmp(action, "attaque") == 0)
        {
            if (((double)(rand() % 100) / 100.0) >= *actionneur_critique)
            {
                *receveur_vie -= *actionneur_degat * 2;
                printf("Critique !\n");
            }
            else if (*receveur_defense)
            {
                *receveur_vie -= 1;
                *receveur_defense = false;
                printf("Defense perdue\n");
            }
            else
            {
                *receveur_vie -= *actionneur_degat;
            }
            printf("Vie du receveur %d\n", *receveur_vie);
        }
}

// --==={  JEU EN COURS  }===--

void partie(int *nombre_de_salle,int *etage)
{
    srand(time(NULL));

    // Activation du jeu

    bool jeu = true;

    // --==={  CHAINES DE CHARACTERE  }===--

    char action[20];

    // --==={  PARAMETTRE JOUEUR  }===--

    char classe[20];
    char pseudo[20];

    int joueur_vie_max;
    int joueur_vie ;

    int joueur_degat;

    int joueur_gold = 0;

    float joueur_critique;

    int joueur_defense_p;
    bool joueur_defense = false;

    // --==={  PARAMETTRE MONSTRE  }===--

    char type_monstre[20];

    int monstre_vie ;
    int monstre_degat ;

    int monstre_gold;

    float monstre_critique;

    int monstre_defense_p;
    bool monstre_defense = false;

    bool monstre_actif = false;

    // --==={  PARAMETTRE NIVEAU  }===--
    char salle_type[20];
    int coffre_gold;
    char continuer[20];


    // --==={  PARAMETTRE ACHAT  }===--
    Achat achat_armes[4] = {
        {
            "Dague en bois",
            5,
            10,
            false
        },
        {
            "Dague en fer",
            10,
            25,
            false
        },
         {
            "Dague Noir",
            25,
            100,
            false
        },
        {
            "Eclipse",
            50,
            300,
            false
        }

    };
    Achat achat_armures[4]={
        {
            "Armure en bois",
            5,
            10,
            false
        },
        {
            "Armure en fer",
            15,
            30,
            false
        },
        {
            "Armure Nor",
            30,
            100,
            false
        },
        {
            "Armure de l'Eclipse",
            50,
            300,
            false
        },

    };

    // Variable tmp aleatoire
    // float r;

    // --==={  INNITATION DU JOUEUR  }===--

    printf("Choisis ton pseudo\n");
    scanf("%19s", pseudo);

    printf("Choisis ta classe joueur : mage, guerrier, assassin\n");
    scanf("%19s", classe);

    classe_joueur(&joueur_vie,&joueur_vie_max,&joueur_degat,&joueur_critique,&joueur_defense_p,classe,&jeu);

    // --==={  JEU  }===--

    printf("Vous passez au %d étage \n",*etage);

    while (jeu == true && joueur_vie > 0 )
    {
        printf("Vous êtes dans la salle %d\n",*nombre_de_salle);

        // --==={  SALLE SPECIAL  }===--

        if (*nombre_de_salle == 10)
        {
            strcpy(salle_type, "Monstre");
        }
        else if(*nombre_de_salle == 5 || *nombre_de_salle == 9)
        {
            boutique(&joueur_degat,&joueur_defense_p,&joueur_gold,achat_armes,achat_armures,&jeu);
            printf("Vous avez %d gold et vous faites %d degats\n",joueur_gold,joueur_degat);
            *nombre_de_salle+=1;
        }
        else{
            salle(salle_type);
        }

        //printf("Salle : %s",salle_type);

        // --==={  SALLE DU MONSTRE  }===--

        if (strcmp(salle_type, "Monstre") == 0){

            // Tour 1 : Monstre ? Oui Non
            if (monstre_actif == false){
                // Choix du monstre
                if (*nombre_de_salle >= 10){
                    selection_boss(type_monstre,&monstre_vie,&monstre_degat,&monstre_critique,&monstre_gold,&(*etage));
                    monstre_actif = true;
                    printf("============================================ \n");
                    printf("==    Arrive à la salle de boss du %s== \n", type_monstre);
                    printf("============================================ \n");
                }else{
                    selection_mob(type_monstre,&monstre_vie,&monstre_degat,&monstre_critique,&monstre_gold,&(*etage));
                    monstre_actif = true;
                    printf("================================ \n");
                    printf("==    Une salle avec %s !     == \n", type_monstre);
                    printf("================================ \n");
                }
            }
            while (monstre_actif == true && joueur_vie >0)
                {

                    printf("Pseudo : %s\nVie : %d\nDefense : %d\n", pseudo, joueur_vie,joueur_defense_p);
                    printf("Monstre : %s\nVie : %d\n", type_monstre, monstre_vie);

                    // Tour du joueur
                    printf("----==={  Tour de %s  }===----\n",pseudo);
                    printf("%s \nAction : Potion, Attaque ou Defendre\n", pseudo);
                    scanf("%s", action);

                    // Tour du Monstre
                    printf("----==={  Tour de %s  }===----\n",type_monstre);
                    printf("Action : Potion, Attaque ou Defendre\n");
                    tour(&joueur_vie,&joueur_vie_max , &joueur_critique, &joueur_degat,&joueur_defense_p ,&joueur_defense,
                        &monstre_vie, &monstre_defense, action);

                    // Action du monstre

                    strcpy(action, "Attaque");
                    tour(&monstre_vie,&monstre_vie, &monstre_critique, &monstre_degat,&monstre_defense_p  ,&monstre_defense,
                        &joueur_vie, &joueur_defense, action);

                    if (monstre_vie <= 0){
                        joueur_gold += monstre_gold;
                        printf("Votre solde est de %d gold \n",joueur_gold);
                        monstre_actif = false;
                    }
                    printf("----==={  Nouveau Round  }===----\n");
                }

            *nombre_de_salle+=1;
            printf("Vie joueur %d\n", joueur_vie);


        }

        // --==={  SALLE DU COFFRE  }===--

        else if (strcmp(salle_type, "Coffre") == 0)
        {

            coffre_gold = rand() % 21;
            printf("=============================== \n");
            printf("== Une salle avec un coffre !== \n");
            printf("=============================== \n");
            printf("Vous avez trouvé %d ! \n",coffre_gold);
            *nombre_de_salle+=1;
            joueur_gold += coffre_gold;


        }

        // --==={  SALLE VIDE  }===--

        else if (strcmp(salle_type, "Vide") == 0)
        {
            printf("=============================== \n");
            printf("==  Cette salle est vide...  == \n");
            printf("=============================== \n");
            *nombre_de_salle+=1;

        }

        // --==={  ERREUR DE SALLE  }===--

        else{

            printf("=============================== \n");
            printf("== Erreur :Probleme de salle == \n");
            printf("=============================== \n");
        }

        printf("Continuer ? (Oui/Non) : \n");
        scanf(" %19s", continuer);
        if (strcmp(continuer, "Non") == 0)
        {
            jeu = false;
        }else{ jeu = true;}

        }

        // --==={  MONTER EN ETAGE  }===--

        if (*nombre_de_salle >= 10){
            *nombre_de_salle = 1;
            *etage = 2;
            printf("Vous passez au %d étage \n",*etage);
        }

        // --==={  FIN DE JEU  }===--

        if (joueur_vie <= 0)
        {
            printf("Le joueur est mort !\n");
        }
        else if (jeu != true)
        {
            printf("Vous avez fermé le jeu !\n");
        }

}

// --==={  MAIN : DEMARAGE DU JEU  }===--

int main()
{
    srand(time(NULL));

    char rejouer;

    do {
        int nombre_de_salle = 1;
        int etage = 1;
        printf("---==={  Bienvennue dans le donjon  }===---\n");
        partie(&nombre_de_salle,&etage);
        printf("Rejouer ? (o/n) : \n");
        scanf(" %c", &rejouer);   // l'espace avant %c est indispensable
    } while (rejouer == 'o' || rejouer == 'O');

    printf("A bientot !\n");
    return 0;

}

#include <stdio.h>
int addition(int a,int b){
        if (a>b)
        {
            return a - b;
        }
        else {
            return b - a;
        };
    }

int main()
{
    printf("Mon premier exercice\n");
    int resultat = addition(48,7);
    printf("Le resultat est %d", resultat);
    int a = 0;
    for (int i = 0; i < 10; ++i){
        a = a + 1;
        printf("Marche \n");
        printf("Test variable %d \n",a);
    }

    return 0;

}

#include <stdio.h>
#include <stdbool.h>

int main() {
    printf("Indtast antallet af tage\n");
    int roof_amount;
    scanf("%d",&roof_amount);
    int roofs[roof_amount][12];
    /* Tagene har 9 parametrer for at holde styr på følgende tal:
     * 0 = x1
     * 1 = y1
     * 2 = x2
     * 3 = y2
     * 4 = laveste y værdi
     * 5 = hældningsretning (0 for venstre, 1 for højre)
     * 6 = naturligt modtaget vand
     * 7 = vand modtaget fra andre tage
     * 8 = nummer indtastet tag (1 for første, 2 for anden osv.)
     * 9 = tildækket x1
     * 10 = tildækket x2
     * 11 = giver den allerede vand til et tag? (0 for nej, 1 for ja)
    */
    int x1 = 0;
    int y1 = 1;
    int x2 = 2;
    int y2 = 3;
    int lowest_y = 4;
    int slope_direction = 5;
    int natural_water = 6;
    int other_water = 7;
    int roof_number = 8;
    int covered_x1 = 9;
    int covered_x2 = 10;
    int giving_water = 11;
    for (int i = 0; i < roof_amount; i++) {
        printf("Indtast koordinaterne for tag %d\n",i+1);
        for (int j = 0; j < 4; j++) {
            scanf("%d",&roofs[i][j]);
        }
    }
    // Udregner hvilken side tagets laveste y værdi er, og derved finder ud af hvilken vej det hælder.
    // Samtidig bliver diverse værdier sat, til brug senere.
    for (int i = 0; i < roof_amount; i++) {
        if (roofs[i][y1] > roofs[i][y2]) {
            roofs[i][lowest_y] = roofs[i][y2];
            roofs[i][slope_direction] = 1;
        }
        else {
            roofs[i][lowest_y] = roofs[i][y1];
            roofs[i][slope_direction] = 0;
        }
        roofs[i][natural_water] = 0;
        roofs[i][other_water] = 0;
        roofs[i][roof_number] = i;
        roofs[i][covered_x1] = roofs[i][0];
        roofs[i][covered_x2] = roofs[i][2];
        roofs[i][giving_water] = 0;
    }

    // Bare en
    char * retning;
    for (int i = 0; i < roof_amount; i++) {
        printf("Tag %d har x,y koordinaterne: %d, %d og %d, %d\n",i+1,roofs[i][x1],roofs[i][y1],roofs[i][x2],roofs[i][y2]);
        if (roofs[i][slope_direction] == 1) {
            retning = "hojre";
        }
        else {
            retning = "venstre";
        }
        printf("Dette tag haelder mod %s.\n",retning);
    }


    // Algorithme til at sortere tagene efter laveste y værdi.
    int temporary_roof_holder[12];
    for (int i = 0; i < roof_amount; i++) {
        for (int j = 0; j < roof_amount; j++) {
            if (roofs[i][lowest_y] < roofs[j][lowest_y]) {
                for (int k = 0; k < 12; k++) {
                    temporary_roof_holder[k] = roofs[j][k];
                }
                for (int k = 0; k < 12; k++) {
                    roofs[j][k] = roofs[i][k];
                    roofs[i][k] = temporary_roof_holder[k];
                }
            }
        }
    }
    printf("Tagene i sorteret orden er:\n");
    for (int i = 0; i < roof_amount; i++) {
        printf("%d, ",roofs[i][roof_number]+1);
    }
    printf("\n");

    // Nu det endelig tid til at finde ud af hvor meget vand hvert tag hver især får.
    int fully_covered = 0;
    for (int i = roof_amount-1; i > -1; i--) {
        for (int j = i+1; j < roof_amount; j++) {
            // Finder ud af om hele tag[i] er dækket af tag[j]
            // Hvis det er, bliver fully_covered = 1 og taget modtager ikke naturligt vand.
            if (roofs[j][x1] < roofs[i][x1] && roofs[i][x1] < roofs[j][x2] && roofs[j][x1] < roofs[i][x2] && roofs[i][x2] < roofs[j][x2]) {
                fully_covered = 1;
            }
            // Finder ud af om venstre x værdi er tildækket
            else if (roofs[j][x1] < roofs[i][x1] && roofs[i][x1] < roofs[j][x2]) {
                if (roofs[i][covered_x1] < roofs[i][x1]+(roofs[j][x2]-roofs[i][x1])) {
                    roofs[i][covered_x1] = roofs[i][x1]+(roofs[j][x2]-roofs[i][x1]);
                }
            }
            // Finder ud af om højre x værdi er tildækket
            else if (roofs[j][x1] < roofs[i][x2] && roofs[i][x2] < roofs[j][x2]) {
                if (roofs[i][covered_x2] > roofs[i][x2]-(roofs[i][x2]-roofs[j][x1])) {
                    roofs[i][covered_x2] = roofs[i][x2]-(roofs[i][x2]-roofs[j][x1]);
                }

            }
            else {
                continue;
            }
            // Her udregnes der om roof[j] skal give vand til roof[i]
            if (roofs[j][giving_water] == 0) {
                if (roofs[j][slope_direction] == 0) {
                    if (roofs[i][x1] < roofs[j][x1] && roofs[j][x1] < roofs[i][x2]) {
                        roofs[i][other_water] = roofs[i][other_water] + roofs[j][natural_water];
                        roofs[i][other_water] = roofs[i][other_water] + roofs[j][other_water];
                        roofs[j][giving_water] = 1;
                    }
                    if (roofs[i][x1] == roofs[j][x1] || roofs[i][x2] == roofs[j][x1]) {
                        roofs[i][other_water] = roofs[i][other_water] + roofs[j][natural_water];
                        roofs[i][other_water] = roofs[i][other_water] + roofs[j][other_water];
                        roofs[j][giving_water] = 1;
                    }
                }
                else {
                    if (roofs[i][x1] < roofs[j][x2] && roofs[j][x2] < roofs[i][x2]) {
                        roofs[i][other_water] = roofs[i][other_water] + roofs[j][natural_water];
                        roofs[i][other_water] = roofs[i][other_water] + roofs[j][other_water];
                        roofs[j][giving_water] = 1;
                    }
                    if (roofs[i][x2] == roofs[j][x2] || roofs[i][x1] == roofs[j][x2]) {
                        roofs[i][other_water] = roofs[i][other_water] + roofs[j][natural_water];
                        roofs[i][other_water] = roofs[i][other_water] + roofs[j][other_water];
                        roofs[j][giving_water] = 1;
                    }
                }
            }
        }
        // Tjekker om roof[i] er fully covered af et andet tag, eller om den bare er covered af flere tag.
        if (roofs[i][covered_x2] < roofs[i][covered_x1] || fully_covered == 1) {
            roofs[i][natural_water] = 0;
        }
        else {
            roofs[i][natural_water] = roofs[i][covered_x2] - roofs[i][covered_x1]; // udregner naturligt fremkommende vand
        }
        fully_covered = 0;
    }
    for (int i = 0; i < roof_amount; i++) {
        for (int j = 0; j < roof_amount; j++) {
            if (roofs[i][roof_number] < roofs[j][roof_number]) {
                for (int k = 0; k < 12; k++) {
                    temporary_roof_holder[k] = roofs[j][k];
                }
                for (int k = 0; k < 12; k++) {
                    roofs[j][k] = roofs[i][k];
                    roofs[i][k] = temporary_roof_holder[k];
                }
            }
        }
    }

    for (int i = 0; i < roof_amount; i++) {
        printf("Tag %d modtager: %d liter vand\n",roofs[i][roof_number]+1,(roofs[i][natural_water]+roofs[i][other_water]));
    }


}
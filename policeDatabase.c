#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DRIVERS 2000
#define MAX_VEHICLES 1000
#define MAX_INFRACTIONS 800

// This struct represents a driver in the database
typedef struct {
    char *license;
    char *name;
    char *street;
    char *city;
    char *province;
} Driver;


// This struct represents a vehicle in the database
typedef struct {
    char *make;
    char *model;
    char *color;
    char *plate;
    unsigned short int year;
    unsigned char reportedStolen;
    Driver *owner;
} Vehicle;


// This struct represents an infraction in the database
typedef struct {
    float amount;
    char *description;
    unsigned short int year;
    unsigned char month;
    unsigned char day;
    unsigned char outstanding;
    Driver *driver;
} Infraction;


// This struct represents the database
typedef struct {
    Driver drivers[MAX_DRIVERS];
    unsigned short int numDrivers;
    Vehicle vehicles[MAX_VEHICLES];
    unsigned short int numVehicles;
    Infraction infractions[MAX_INFRACTIONS];
    unsigned short int numInfractions;
} PoliceDatabase;


// ********************************************
// WRITE ALL YOUR FUNCTIONS AND PROCEDURES HERE
// ********************************************

int registerDriver(PoliceDatabase *pd, char *dl, char *name, char *st, char *city, char *prov){
    if(MAX_DRIVERS > pd->numDrivers){
        pd->drivers[pd->numDrivers].license = dl;
        pd->drivers[pd->numDrivers].name = name;
        pd->drivers[pd->numDrivers].street = st;
        pd->drivers[pd->numDrivers].city = city;
        pd->drivers[pd->numDrivers].province = prov;
        pd->numDrivers ++;
        return 1;
    }
    return 0;
}

int registerVehicle(PoliceDatabase *pd, char *make, char *model, unsigned short int year,
                    char *color, char *plate, char *DL){
    if(MAX_VEHICLES > pd->numVehicles){
        pd->vehicles[pd->numVehicles].make = make;
        pd->vehicles[pd->numVehicles].model = model;
        pd->vehicles[pd->numVehicles].year = year;
        pd->vehicles[pd->numVehicles].color = color;
        pd->vehicles[pd->numVehicles].plate = plate;
        for(int i = 0; i < pd->numDrivers; i++){
            if(pd->drivers[i].license == DL){
                pd->vehicles[pd->numVehicles].owner = &pd->drivers[i];
            }
        }
        pd->numVehicles ++;
        return 1;
    }
    return 0;
}


int unregisterVehicle(PoliceDatabase *pd, char *plate) {
    for (int i = 0; i < pd->numVehicles; i++) {
        if (pd->vehicles[i].plate == plate) {
            for (int j = i; j < pd->numVehicles - 1; j++) {
                pd->vehicles[j] = pd->vehicles[j + 1];
            }
            pd->numVehicles --;
            return 1;
        }
    }
    return 0;
}


void pay(Infraction *inf){
    inf->outstanding = 'F';
}

int changeOwner(PoliceDatabase *pd, char *plate, char *DL){
    for (int i = 0; i < pd->numDrivers; i++){
        for (int j = 0; j < pd->numVehicles; j++) {
            if (pd->drivers[i].license == DL && pd->vehicles[j].plate == plate){
                pd->vehicles[j].owner = &pd->drivers[i];
                return 1;
            }
        }
    }
    return 0;
}

int reportStolen(PoliceDatabase *pd, char *plate){
    for (int i = 0; i < pd->numVehicles; i++) {
        if (pd->vehicles[i].plate == plate){
            pd->vehicles[i].reportedStolen = 'T';
            return 1;
        }
    }
    return 0;
}

Infraction *
issueInfraction(PoliceDatabase *pd, char *DL, float amount, char *description, unsigned short int year, unsigned char month,
                unsigned char day){
    if (pd->numInfractions < MAX_INFRACTIONS) {
        static Infraction inf;
        for (int i = 0; i < pd->numDrivers; i++) {
            if (pd->drivers[i].license == DL) {
                inf.driver = &pd->drivers[i];
                inf.amount = amount;
                inf.description = description;
                inf.year = year;
                inf.month = month;
                inf.day = day;
                inf.outstanding = 'T';
                pd->infractions[pd->numInfractions] = inf;
                pd->numInfractions ++;

                //printf("the issue infraction driver %s has outstanding amount $ %0.2f\n", inf.driver->name, inf.amount);

                return &inf;
            }
        }
    }
    return NULL;
}

int hasOutstandingInfractions(PoliceDatabase *pd, Driver *dr){
    for (int i = 0; i < pd->numInfractions; i++){
        if (pd->infractions[i].driver == dr && pd->infractions[i].outstanding == 'T')
            return 1;
    }
    return 0;
}

int hasInfractions(PoliceDatabase *pd, Driver *dr){
    for (int i = 0; i < pd->numInfractions; i++){
        if (pd->infractions[i].driver == dr)
            return 1;
    }
    return 0;
}

int shouldStopVehicle(PoliceDatabase *pd, char *plate){
    for (int i = 0; i < pd->numVehicles; i++){
        if (pd->vehicles[i].plate == plate){
            if (pd->vehicles[i].reportedStolen == 'T' ||
                hasOutstandingInfractions(pd,pd->vehicles[i].owner) == 1)
                return 1;
        }
    }
    return 0;
}

void showInfractionsFor(PoliceDatabase *pd, Driver *dr){
    int count = 0;
    for (int i = 0; i < pd->numInfractions; i++){
        if ((pd->infractions[i].driver->license = dr->license)){
            if (hasOutstandingInfractions(pd,dr) == 1){
                printf("$%f infraction on %d/%c/%c [PAID IN FULL]\n", pd->infractions[i].amount, pd->infractions[i].year, pd->infractions[i].month, pd->infractions[i].day);
            } else {
                printf("$%f infraction on %d/%c/%c [OUTSTANDING]\n", pd->infractions[i].amount, pd->infractions[i].year, pd->infractions[i].month, pd->infractions[i].day);
                count ++;
            }
        }
    }
    printf("Total outstanding infractions = %d\n", count);
}

int cleanDrivers(PoliceDatabase *pd, Driver **dr){
    int count = 0;
    *dr = malloc(20* sizeof(Driver));
    for (int i; i<pd->numDrivers; i++){
        if (hasInfractions (pd,&pd->drivers[i]) == 1) {
            (*dr)[count] = pd->drivers[i];
            count ++;
        }
    }
    return count;
}

void showInfractionReport(PoliceDatabase *pd){
    int idx = 0;
    char **name = NULL;
    char **DL = NULL;
    int *number = 0;
    float *paidAmount = 0;

//    for (int i; i<pd->numDrivers; i++){
//        if (hasInfractions (pd,&pd->drivers[i]) == 1) {
//            name[idx] = pd->infractions[i].driver->name;
//            DL[idx] = pd->infractions[i].driver->license;
//            number[idx] ++;
//            paidAmount[idx] += pd->infractions[i].amount;
//            idx++;
//        }
//    }

    for (int i = 0; i < pd->numInfractions; i++){
        if(pd->infractions[i].outstanding == 'F'){
            for (int j = 0; j < idx+1; j++){
                if (pd->infractions[i].driver->license == DL[j]){
                    number[j] ++;
                    paidAmount[j] += pd->infractions[i].amount;
                }else{
                    name[idx] = pd->infractions[i].driver->name;
                    DL[idx] = pd->infractions[i].driver->license;
                    number[idx] ++;
                    paidAmount[idx] += pd->infractions[i].amount;
                    idx++;
                }
            }
        }
    }

    for (int i = 0; i < idx+1; i++){
        printf("%c: %d infractions, total paid = $ %f\n",*name[i], number[i], paidAmount[i]);
    }
}

// HERE IS THE TEST CODE ... YOU MUST NOT ALTER THIS FUNCTION IN ANY WAY
int main() {
    // Make a new database and add some vehicles/owners etc..
    PoliceDatabase  pdb;

    pdb.numDrivers = 0;
    pdb.numVehicles = 0;
    pdb.numInfractions = 0;

    registerDriver(&pdb, "L1567-34323-84980", "Matt Adore", "1323 Kenaston St.", "Gloucester", "ON");
    registerDriver(&pdb, "L0453-65433-87655", "Bob B. Pins", "32 Rideau Rd.", "Greely", "ON");
    registerDriver(&pdb, "L2333-45645-54354", "Stan Dupp", "1355 Louis Lane", "Gloucester", "ON");
    registerDriver(&pdb, "L1234-35489-99837", "Ben Dover","2348 Walkley Rd.", "Ottawa", "ON");
    registerDriver(&pdb, "L8192-87498-27387", "Patty O'Lantern", "2338 Carling Ave.", "Nepean", "ON");
    registerDriver(&pdb, "L2325-45789-35647", "Ilene Dover", "287 Bank St.", "Ottawa", "ON");
    registerDriver(&pdb, "L1213-92475-03984", "Patty O'Furniture","200 St. Laurant Blvd.", "Ottawa", "ON");
    registerDriver(&pdb, "L1948-87265-34782", "Jen Tull", "1654 Stonehenge Cres.", "Ottawa", "ON");
    registerDriver(&pdb, "L0678-67825-83940", "Jim Class", "98 Oak Blvd.", "Ottawa", "ON");
    registerDriver(&pdb, "L0122-43643-73286", "Mark Mywords", "3 Third St.", "Ottawa", "ON");
    registerDriver(&pdb, "L6987-34532-43334", "Bob Upandown", "434 Gatineau Way", "Hull", "QC");
    registerDriver(&pdb, "L3345-32390-23789", "Carrie Meehome", "123 Thurston Drive", "Kanata", "ON");
    registerDriver(&pdb, "L3545-45396-88983", "Sam Pull", "22 Colonel By Drive", "Ottawa", "ON");
    registerDriver(&pdb, "L1144-26783-58390", "Neil Down", "17 Murray St.", "Nepean", "ON");
    registerDriver(&pdb, "L5487-16576-38426", "Pete Reedish", "3445 Bronson Ave.", "Ottawa", "ON");

    registerVehicle(&pdb, "Honda", "Civic", 2015, "yellow", "W3EW4T", "L0453-65433-87655");
    registerVehicle(&pdb, "Pontiac","Grand Prix",2007,"dark green","GO SENS", "L0453-65433-87655");
    registerVehicle(&pdb, "Mazda", "RX-8", 2004, "white", "OH YEAH", "L2333-45645-54354");
    registerVehicle(&pdb, "Nissan","Altima",2017,"bergundy", "Y6P9O7", "L1234-35489-99837");
    registerVehicle(&pdb, "Saturn", "Vue", 2002, "white", "9R6P2P", "L2325-45789-35647");
    registerVehicle(&pdb, "Honda", "Accord", 2018, "gray", "7U3H5E", "L2325-45789-35647");
    registerVehicle(&pdb, "Chrysler", "PT-Cruiser", 2006, "gold", "OLDIE", "L2325-45789-35647");
    registerVehicle(&pdb, "Nissan", "Cube", 2010, "white", "5Y6K8V", "L1948-87265-34782");
    registerVehicle(&pdb, "Porsche", "959", 1989, "silver", "CATCHME", "L0678-67825-83940");
    registerVehicle(&pdb, "Kia", "Soul", 2018, "red", "J8JG2Z", "L0122-43643-73286");
    registerVehicle(&pdb, "Porsche", "Cayenne", 2014, "black", "EXPNSV", "L6987-34532-43334");
    registerVehicle(&pdb, "Nissan", "Murano", 2010, "silver", "Q2WF6H", "L3345-32390-23789");
    registerVehicle(&pdb, "Honda", "Element", 2008, "black", "N7MB5C", "L3545-45396-88983");
    registerVehicle(&pdb, "Toyota", "RAV-4", 2010, "green", "R3W5Y7", "L3545-45396-88983");
    registerVehicle(&pdb, "Toyota", "Celica", 2006, "red", "FUNFUN", "L5487-16576-38426");

    pay(issueInfraction(&pdb, "L0453-65433-87655", 75, "Illegal U-Turn", 2012, 6, 14));
    pay(issueInfraction(&pdb, "L2325-45789-35647", 175, "Speeding in excess of 20km", 2013, 2, 2));
    pay(issueInfraction(&pdb, "L2325-45789-35647", 75, "Illegal U-Turn", 2014, 3, 22));
    pay(issueInfraction(&pdb, "L2325-45789-35647", 150, "Wrong way up 1-way street", 2016, 6, 14));
    issueInfraction(&pdb, "L2325-45789-35647", 500, "Running red light", 2019, 9, 12);
    pay(issueInfraction(&pdb, "L0678-67825-83940", 100, "Failure to signal", 2010, 2, 3));
    pay(issueInfraction(&pdb, "L0678-67825-83940", 250, "Speeding in excess of 30km", 2012, 9, 6));
    pay(issueInfraction(&pdb, "L0678-67825-83940", 250, "Speeding in excess of 30km", 2017, 7, 7));
    issueInfraction(&pdb, "L0678-67825-83940", 350, "Speeding in excess of 40km", 2019, 11, 3);
    issueInfraction(&pdb, "L0678-67825-83940", 350, "Speeding in excess of 40km", 2020, 1, 1);
    issueInfraction(&pdb, "L6987-34532-43334", 500, "Running red light", 2019, 7, 2);
    pay(issueInfraction(&pdb, "L3545-45396-88983", 75, "Illegal U-Turn", 2009, 3, 4));
    pay(issueInfraction(&pdb, "L3545-45396-88983", 175, "Speeding in excess of 20km", 2013, 5, 4));
    issueInfraction(&pdb, "L1144-26783-58390", 175, "Speeding in excess of 20km", 2014, 10, 1);
    issueInfraction(&pdb, "GARBAGE ID", 100, "Loving C Programming too much", 2019, 11, 24);


    printf("Here are the drivers:\n");
    for (int i=0; i<pdb.numDrivers; i++)
        printf("%s %s living at %s, %s, %s\n", pdb.drivers[i].license, pdb.drivers[i].name,
               pdb.drivers[i].street, pdb.drivers[i].city, pdb.drivers[i].province);


    printf("\nHere are the vehicles:\n");
    for (int i=0; i<pdb.numVehicles; i++)
        printf("A %s %d %s %s with plate %s\n", pdb.vehicles[i].color, pdb.vehicles[i].year,
               pdb.vehicles[i].make, pdb.vehicles[i].model, pdb.vehicles[i].plate);


    printf("\nRemoving 3 vehicles (i.e., W3EW4T, FUNFUN and CATCHME) ...\n");
    unregisterVehicle(&pdb, "W3EW4T");
    unregisterVehicle(&pdb, "FUNFUN");
    unregisterVehicle(&pdb, "CATCHME");


    printf("\nHere are the remaining vehicles:\n");
    for (int i=0; i<pdb.numVehicles; i++)
        printf("A %s %d %s %s with plate %s\n", pdb.vehicles[i].color, pdb.vehicles[i].year,
               pdb.vehicles[i].make, pdb.vehicles[i].model, pdb.vehicles[i].plate);


    printf("\nHere is the owner of the Honda Accord:\n");
    printf("%s\n", pdb.vehicles[4].owner->name);
    printf("Ilene Dover sells the car to Sam Pull, here is the owner now:\n");
    changeOwner(&pdb, "7U3H5E", "L3545-45396-88983");
    printf("%s\n", pdb.vehicles[4].owner->name);


    printf("\nHere are the stolen cars at this time:\n");
    for (int i=0; i<pdb.numVehicles; i++)
        if (pdb.vehicles[i].reportedStolen)
            printf("A %s %d %s %s with plate %s\n", pdb.vehicles[i].color, pdb.vehicles[i].year,
                   pdb.vehicles[i].make, pdb.vehicles[i].model, pdb.vehicles[i].plate);


    printf("\nReport some cars as stolen...\n");
    reportStolen(&pdb, "OH YEAH");
    reportStolen(&pdb, "OLDIE");
    reportStolen(&pdb, "LUVJAVA"); // This one won’t work, but shouldn’t crash


    printf("\nHere are the stolen cars now:\n");
    for (int i=0; i<pdb.numVehicles; i++)
        if (pdb.vehicles[i].reportedStolen)
            printf("A %s %d %s %s with plate %s\n", pdb.vehicles[i].color, pdb.vehicles[i].year,
                   pdb.vehicles[i].make, pdb.vehicles[i].model, pdb.vehicles[i].plate);


    printf("\nVehicles that should be stopped:\n");
    for (int i=0; i<pdb.numVehicles; i++)
        if (shouldStopVehicle(&pdb, pdb.vehicles[i].plate))
            printf("A %s %d %s %s with plate %s\n", pdb.vehicles[i].color, pdb.vehicles[i].year,
                   pdb.vehicles[i].make, pdb.vehicles[i].model, pdb.vehicles[i].plate);


    printf("\nHere are all the clean drivers:\n");
    Driver *cleanOnes = NULL;
    int numClean = cleanDrivers(&pdb, &cleanOnes);
    for (int i=0; i<numClean; i++)
        printf("%s %s living at %s, %s, %s\n", cleanOnes[i].license, cleanOnes[i].name,
               cleanOnes[i].street, cleanOnes[i].city, cleanOnes[i].province);


    printf("\nHere is the infraction report:\n");
    showInfractionReport(&pdb);


    printf("\nHere is the status of Jim Class:\n");
    showInfractionsFor(&pdb, "L0678-67825-83940");
}

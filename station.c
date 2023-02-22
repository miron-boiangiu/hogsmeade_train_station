#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "station.h"


/* Creeaza o gara cu un numar fix de peroane.
 * 
 * platforms_no: numarul de peroane ale garii
 *
 * return: gara creata
 */
TrainStation* open_train_station(int platforms_no) {
    TrainStation* gara = calloc(1, sizeof(TrainStation));
    gara->platforms_no = platforms_no;
    gara->platforms = calloc(platforms_no, sizeof(Train*));
    return gara;
}


/* Elibereaza memoria alocata pentru gara.
 *
 * station: gara existenta
 */
void close_train_station(TrainStation *station) {
    if(station == NULL)
        return;
    for(int i = 0; i<station->platforms_no; i++){
        if(station->platforms[i] != NULL){
            leave_train(station, i);
        }
    }
    free(station->platforms);
    free(station);
}


/* Afiseaza trenurile stationate in gara.
 *
 * station: gara existenta
 * f: fisierul in care se face afisarea
 */
void show_existing_trains(TrainStation *station, FILE *f) {
    if (station == NULL)
        return;
    //fprintf(f, "Printing %d platforms.\n", station->platforms_no);
    for(int platform_no = 0; platform_no<station->platforms_no; platform_no++){
        fprintf(f, "%d: ", platform_no);
        if(station->platforms[platform_no] != NULL){
            Train* train = station->platforms[platform_no];
            fprintf(f, "(%d)", train->locomotive_power);
            if(train->train_cars != NULL){
                TrainCar* trainCar = train->train_cars;
                do{
                    fprintf(f, "-|%d|", trainCar->weight);
                    trainCar = trainCar->next;
                }while(trainCar!=NULL);
            }
        }
        fprintf(f, "\n");
    }
}


/* Adauga o locomotiva pe un peron.
 * 
 * station: gara existenta
 * platform: peronul pe care se adauga locomotiva
 * locomotive_power: puterea de tractiune a locomotivei
 */
void arrive_train(TrainStation *station, int platform, int locomotive_power) {
    if(station == NULL)
        return;
    if(platform < station->platforms_no && 0<= platform){
        if(station->platforms[platform] == NULL){
            Train* train = calloc(1, sizeof(Train));
            train->locomotive_power = locomotive_power;
            train->train_cars = NULL;
            station->platforms[platform] = train;
        }
    }
}



void remove_all_traincars(TrainCar* traincar){
    if(traincar == NULL)
        return;
    remove_all_traincars(traincar->next);
    free(traincar);
}
/* Elibereaza un peron.
 * 
 * station: gara existenta
 * platform: peronul de pe care pleaca trenul
 */
void leave_train(TrainStation *station, int platform) {
    if(station == NULL)
        return;
    if(0 <= platform && platform < station->platforms_no){
        if(station->platforms[platform] != NULL){
            Train* train_to_be_deleted = station->platforms[platform];
            remove_all_traincars(train_to_be_deleted->train_cars);
            free(train_to_be_deleted);
            station->platforms[platform] = NULL;
        }
    }
}


/* Adauga un vagon la capatul unui tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului adaugat
 */
void add_train_car(TrainStation *station, int platform, int weight) {
    if(station == NULL)
        return;
    if(weight < 0)
        return;
    if(0 <= platform && platform < station->platforms_no && station->platforms[platform] != NULL){
        Train* train = station->platforms[platform];
        TrainCar* newTrainCar= calloc(1, sizeof(TrainCar));
        newTrainCar->weight = weight;
        newTrainCar->next = NULL;

        if(train->train_cars == NULL){
            train->train_cars = newTrainCar;
            return;
        }
        TrainCar* currentTrainCar = train->train_cars;
        while(currentTrainCar->next != NULL){
            currentTrainCar = currentTrainCar->next;
        }
        currentTrainCar->next = newTrainCar;
    }
}


/* Scoate vagoanele de o anumita greutate dintr-un tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului scos
 */
void remove_train_cars(TrainStation *station, int platform, int weight) {
    if(station == NULL)
        return;
    if(weight < 0)
        return;
    if(0 <= platform && platform < station->platforms_no && station->platforms[platform] != NULL){
        Train* train = station->platforms[platform];
        if(train->train_cars == NULL){
            return;
        }
        TrainCar* currentTrainCar = train->train_cars;
        while(currentTrainCar->weight == weight){
            if(currentTrainCar->next == NULL){
                free(currentTrainCar);
                train->train_cars = NULL;
                return;
            }
            else{
                train->train_cars = currentTrainCar->next;
                free(currentTrainCar);
                currentTrainCar = train->train_cars;
            }
        }
        TrainCar* prevTrainCar;
        while(currentTrainCar != NULL){
            if(currentTrainCar->weight == weight){
                prevTrainCar->next = currentTrainCar->next;
                free(currentTrainCar);
                currentTrainCar = prevTrainCar->next;
                continue;
            }
            prevTrainCar = currentTrainCar;
            currentTrainCar = currentTrainCar->next;
        }
    }
}

int count_train_cars(Train* train){
    if(train == NULL)
        return -1;
    if(train->train_cars == NULL){
        return 0;
    }
    int number = 0;
    TrainCar* traincar = train->train_cars;
    while(traincar != NULL){
        number++;
        traincar = traincar->next;
    }
    return number;
}
/* Muta o secventa de vagoane dintr-un tren in altul.
 * 
 * station: gara existenta
 * platform_a: peronul pe care se afla trenul de unde se scot vagoanele
 * pos_a: pozitia primului vagon din secventa
 * cars_no: lungimea secventei
 * platform_b: peronul pe care se afla trenul unde se adauga vagoanele
 * pos_b: pozitia unde se adauga secventa de vagoane
 */
void move_train_cars(TrainStation *station, int platform_a, int pos_a, 
                                int cars_no, int platform_b, int pos_b) {
                                
    //Prepare yourself for an absolutely disgusting piece of code...which works!
    //Checking for a LOT of stupid user-related errors...
    //Seriously, did you really have to check for all of these?
    //It physically hurt to write this function.
    //Does the station exist?
    if(station == NULL)
        return;
    //Are the platforms correct numbers?
    if(!(0 <= platform_a && platform_a < station->platforms_no))
        return;
    if(!(0 <= platform_b && platform_b < station->platforms_no))
        return;
    //Are the trains at the platforms?
    if(station->platforms[platform_a] == NULL)
        return;
    if(station->platforms[platform_b] == NULL)
        return;
    
    Train* platform_a_train = station->platforms[platform_a];
    Train* platform_b_train = station->platforms[platform_b];
    //Check if the positions make sense:
    if(cars_no > count_train_cars(platform_a_train))
        return;
    if(cars_no <= 0)
        return;
    if(!(1 <= pos_a && pos_a <= count_train_cars(platform_a_train)))
        return;
    if(!(1 <= pos_b && pos_b <= count_train_cars(platform_b_train)+1))
        return;
    if(pos_a + cars_no - 1 > count_train_cars(platform_a_train))
        return;
    if(station->platforms[platform_a]->train_cars == NULL)
        return;
    int pos_a_index = 1;
    TrainCar* platform_a_traincar_pos_a = platform_a_train->train_cars;
    TrainCar* platform_b_traincar_pos_b = platform_b_train->train_cars;
    TrainCar* prev = NULL;

    //Pick up the traincars that we have to move.

    while(pos_a_index < pos_a){
        prev = platform_a_traincar_pos_a;
        platform_a_traincar_pos_a = platform_a_traincar_pos_a->next;
        pos_a_index++;
    }
    TrainCar* last_traincar_to_pick_up = platform_a_traincar_pos_a;
    for(int i = 1; i<cars_no; i++){
        last_traincar_to_pick_up = last_traincar_to_pick_up->next;
    }
    if(prev == NULL){
        platform_a_train->train_cars = last_traincar_to_pick_up->next;
    }
    else{
        prev->next = last_traincar_to_pick_up->next;
    }
    last_traincar_to_pick_up->next = NULL;
    //Let's see where we need to put them.

    int pos_b_index = 1;
    prev = NULL;
    while(pos_b_index < pos_b){
        prev = platform_b_traincar_pos_b;
        platform_b_traincar_pos_b = platform_b_traincar_pos_b->next;
        pos_b_index++;
    }
    if(pos_b == 1){
        last_traincar_to_pick_up->next = platform_b_train->train_cars;
        platform_b_train->train_cars = platform_a_traincar_pos_a;
        return;
    }
    //Now we put them there.
    prev->next = platform_a_traincar_pos_a;
    //Now we rejoin the last traincar from the just-pasted ones to the remaining ones from platform b.
    last_traincar_to_pick_up->next = platform_b_traincar_pos_b;
}



int traincar_sequence_sum(Train* train, int pos, int how_many){
    int sum = 0;
    if(train->train_cars == NULL)
        return sum;
    TrainCar* traincar = train->train_cars;
    for(int i = 0; i<pos; i++){
        traincar = traincar->next;
    }
    for(int i = 0; i<how_many; i++){
        sum += traincar->weight;
        traincar = traincar->next;
    }
    return sum;
}

/* Gaseste trenul cel mai rapid.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_express_train(TrainStation *station) {
    if(station == NULL)
        return -1;
    int fastest_platform = 0;
    int biggest_difference = 0;
    for(int i = 0; i<station->platforms_no; i++){
        Train* current_train = station->platforms[i];
        if(current_train!=NULL){
            int current_sum = traincar_sequence_sum(current_train, 0, count_train_cars(current_train));
            if(current_train->locomotive_power - current_sum > biggest_difference){
                biggest_difference = current_train->locomotive_power- current_sum;
                fastest_platform = i;
            }
        }
    }
    return fastest_platform;
}


/* Gaseste trenul supraincarcat.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_overload_train(TrainStation *station) {
    if(station == NULL)
        return -1;
    for(int i = 0; i<station->platforms_no; i++){
        Train* current_train = station->platforms[i];
        if(current_train!=NULL){
            int current_sum = traincar_sequence_sum(current_train, 0, count_train_cars(current_train));
            if(current_train->locomotive_power < current_sum){
                return i;
            }
        }
    }
    return -1;
}


/* Gaseste trenul cu incarcatura optima.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_optimal_train(TrainStation *station) {
    if(station == NULL)
        return -1;
    int fastest_platform = -1;
    int smallest_difference = -1;
    for(int i = 0; i<station->platforms_no; i++){
        Train* current_train = station->platforms[i];
        if(current_train!=NULL){
            int current_sum = traincar_sequence_sum(current_train, 0, count_train_cars(current_train));
            if(current_train->locomotive_power < current_sum)
                continue;
            if(current_train->locomotive_power - current_sum < smallest_difference || smallest_difference == -1){
                smallest_difference = current_train->locomotive_power- current_sum;
                fastest_platform = i;
            }
        }
    }
    return fastest_platform;
}


/* Gaseste trenul cu incarcatura nedistribuita bine.
 * 
 * station: gara existenta
 * cars_no: lungimea secventei de vagoane
 *
 * return: peronul pe care se afla trenul
 */
int find_heaviest_sequence_train(TrainStation *station, int cars_no, TrainCar **start_car) {
    //I'm writing this function at 2AM, I'm shocked I can still focus.
    if(station == NULL)
        return -1;
    int fastest_platform = -1;
    int heaviest_sequence = 0;
    *start_car = NULL;
    for(int i = 0; i<station->platforms_no; i++){
        Train* current_train = station->platforms[i];
        if(current_train!=NULL){
            int current_heaviest_sequence = 0;
            int current_sum = 0;
            TrainCar* current_traincar = NULL;
            if(count_train_cars(current_train) < cars_no)
                continue;
            for(int j = 0; j<=count_train_cars(current_train)-cars_no; j++){
                current_sum = traincar_sequence_sum(current_train, j, cars_no);
                if(current_sum > current_heaviest_sequence){
                    current_heaviest_sequence = current_sum;
                    current_traincar = current_train->train_cars;

                    for(int k = 0; k<j; k++){
                        current_traincar = current_traincar->next;
                    }

                }
            }
            if(current_heaviest_sequence > heaviest_sequence){
                heaviest_sequence = current_heaviest_sequence;
                fastest_platform = i;
                *start_car = current_traincar;
            }
        }
    }
    return fastest_platform; //Not really fastest, but I started
                             //from find_express_train(), so..
}


int positionOfHeaviestTrainCar(Train* train){
    TrainCar* trainCar = train->train_cars;
    int pos = -1;
    int weight = 0;
    int index = 0;
    while(trainCar != NULL){
        if(trainCar->weight > weight){
            weight = trainCar->weight;
            pos = index;
        }
        trainCar = trainCar->next;
        index++;
    }
    return pos;
}
/* Ordoneaza vagoanele dintr-un tren in ordinea descrescatoare a greutatilor.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 */
void order_train(TrainStation *station, int platform) {
    //Checking if anyone is trying to screw us over.
    if(platform < 0 || platform > station->platforms_no-1)
        return;
    if(station->platforms[platform] == NULL)
        return;
    if(station->platforms[platform]->train_cars == NULL)
        return;
    
    TrainCar* newSequence = NULL;
    TrainCar* lastInNewSequence = NULL; //<- So I don't have to keep iterating over it like a madman.
    //We will move them one by one to the newSequence, until we form
    //an entirely new line of  traincars, then we couple it to the train. :)
    Train* currentTrain = station->platforms[platform];
    while(currentTrain->train_cars != NULL){
        int posToRemove =  positionOfHeaviestTrainCar(currentTrain);
        TrainCar* trainCarToRemove = NULL;
        if(posToRemove == 0){
            trainCarToRemove = currentTrain->train_cars;
            currentTrain->train_cars = currentTrain->train_cars->next;
            trainCarToRemove->next = NULL;
            if(newSequence==NULL){
                newSequence = trainCarToRemove;
                lastInNewSequence = trainCarToRemove;
            }
            else{
                lastInNewSequence->next = trainCarToRemove;
                lastInNewSequence = trainCarToRemove;
            }
        }
        else{
            trainCarToRemove = currentTrain->train_cars;
            TrainCar* previousTrainCar = NULL;
            for(int i = 0; i<posToRemove; i++){
                previousTrainCar = trainCarToRemove;
                trainCarToRemove = trainCarToRemove->next;
            }
            previousTrainCar->next = trainCarToRemove->next;
            trainCarToRemove->next = NULL;
            if(newSequence==NULL){
                newSequence = trainCarToRemove;
                lastInNewSequence = trainCarToRemove;
            }
            else{
                lastInNewSequence->next = trainCarToRemove;
                lastInNewSequence = trainCarToRemove;
            }
        }
    }
    currentTrain->train_cars = newSequence;
}

int sumOfAllTraincarsExceptOne(Train* train, int pos){
    //traincar_sequence_sum() was the best investment of my life.
    int sum = 0;
    if(pos!=0)
        sum += traincar_sequence_sum(train, 0, pos);
    
    if(pos != count_train_cars(train)-1)
        sum += traincar_sequence_sum(train, pos+1, count_train_cars(train)-pos-1);
    
    return sum;
}
/* Scoate un vagon din trenul supraincarcat.
 * 
 * station: gara existenta
 */
void fix_overload_train(TrainStation *station) {
    //One...more...function.....
    if(station == NULL)
        return;
    
    int posOfOverloadedTrain = find_overload_train(station);
    if(posOfOverloadedTrain == -1)
        return;

    /*
    We want to check if, by removing a traincar, we get the maximum amount
    of weight moved. In order to do this, we'll remove each one of them
    one by one and check if the train can now move and if it's the
    best option so far.
    Why have I now started to explain functions here instead of the README?
    I don't know, I'm tired, maybe I'm trying to tell myself what I have to do.
    */
    int heaviestMovableWeight = 0;
    int posToRemove = -1;
    Train* train = station->platforms[posOfOverloadedTrain];
    TrainCar* currentTrainCar = train->train_cars;
    int index = 0;
    while(currentTrainCar!=NULL){
        int currentSum = sumOfAllTraincarsExceptOne(train, index);
        if(currentSum <= train->locomotive_power){
            if(heaviestMovableWeight < currentSum){
                posToRemove = index;
                heaviestMovableWeight = currentSum;
            }
        }
        index++;
        currentTrainCar = currentTrainCar->next;
    }
    if(posToRemove == 0){
        TrainCar* trainCarToRemove = train->train_cars;
        train->train_cars = train->train_cars->next;
        free(trainCarToRemove);
    }
    else{
        TrainCar* prev = NULL;
        TrainCar* trainCarToRemove = train->train_cars;
        for(int i = 0; i<posToRemove; i++){
            prev = trainCarToRemove;
            trainCarToRemove = trainCarToRemove->next;
        }
        prev->next = trainCarToRemove->next;
        free(trainCarToRemove);
    }
}

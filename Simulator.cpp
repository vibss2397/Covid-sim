#include <cstdlib>
#include <stdio.h>
#include <omp.h>
#include <iostream>
#include "TestingDatabase.h"
#include "Person.hpp"
#include "Graph.hpp"
#include "ChartWriter.hpp"

const int NUM_PERSONS = 1000; //total people number
const float SIM_DURATION = 120.0f; //how many units of time do we run the simulation for?
const float SIM_RATE = 1.0f; //length of time between each time frame of the simulation
const float CONNECTION_PROB = 0.005f; //the connection probability
const int MASK_PROB = 85; //percentage of how many people wear mask on campus
const float PERCENT_INFECTED_AT_START = 0.1f; //percentatge of how may people are infected at start

int main(int argc, char* argv[])
{
    std::vector<Person*> people(NUM_PERSONS);//initialize a people array
    int numTestsPerPerson = SIM_DURATION/SIM_RATE;
    TestingDatabase tb = TestingDatabase(NUM_PERSONS, numTestsPerPerson);
	ChartWriter chartWriter(NUM_PERSONS);//for output
    
    // Figure out one person is infected/safe or mask on/off
    //comment: this can't do in parallel since we want to ensure the percentage
    int i;
    for (i = 0; i < NUM_PERSONS; i++)
    {
        Person *person = new Person(i);
        float temp = ((float)rand()*100)/(float) RAND_MAX;
        bool isInfected =  temp< PERCENT_INFECTED_AT_START;
        if(isInfected){
           person->covid_state[0] = 1;
           person->covid_state[1] = 1;
        }
        people[i] = person;
        bool TrueFalse = (rand() % 100) < MASK_PROB;
        if(TrueFalse){
            person->set_mask();
        }
    }
	
    //initialize the graph g
    Graph g = Graph(people, CONNECTION_PROB);

    float time = 0.0f; //current time
    while(time < SIM_DURATION){
        int i;
        //update the states of each person for the current time frame
        #pragma omp parallel for shared (people, g) private (i) // uncomment this line works only comment it for testing!
        for(i = 0; i < NUM_PERSONS; i++) {
            //push back this person's covid state from the previous time frame,
            //making room for their updated state in the current time frame.
            //If they have covid, this includes determining whether they have recovered or died, etc.
            people[i]->update_covid_state();
            
            //determine if this person is being infected in the current time frame based on the covid states of their neighbors from the previous time frame (i.e. person.covid_state[0])
            people[i]->calculate_covid(g);
            
            //update the testing database given the current state of this person (i.e. person.covid_state[1])
            if (people[i]->covid_state[0] != 4 && people[i]->covid_state[0] != -1)
                bool testResult = tb.test_node(people[i]->bu_id, people[i]->covid_state[1]);
        }  
        // Update chart data
        chartWriter.updateFromData(time, people, NUM_PERSONS, tb);

        // Print out covid state numbers
        std::cout << "Time: " << time << " / " << SIM_DURATION << "\tHealthy: " << chartWriter.healthyCounts[4] << "\tInfected: " << chartWriter.infectedCounts[4] << "\tRecovered: " << chartWriter.recoveredCounts[4] << "\tDead: " << chartWriter.deadCounts[4] << "\n";

        time += SIM_RATE;
    }
	
	// Output charts
	chartWriter.writeAllCharts();
	chartWriter.cleanup();
	
	// Open html viewer
	chartWriter.openWebpage();
}


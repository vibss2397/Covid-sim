#include <cstdlib>
#include <stdio.h>
#include <omp.h>
#include <iostream>
#include "TestingDatabase.h"
#include "Person.hpp"
#include "Graph.hpp"
#include "ChartWriter.hpp"

const int NUM_PERSONS = 16; //total people number
const float SIM_DURATION = 2.0f; //how many units of time do we run the simulation for?
const float SIM_RATE = 1.0f; //length of time between each time frame of the simulation
const float CONNECTION_PROB = 0.8f; //the connection probability
const int MASK_PROB = 50; //percentage of how many people wear mask on campus

int main(int argc, char* argv[])
{
    std::vector<Person> people(NUM_PERSONS);
    // we should still working on the graph class
    int numTestsPerPerson = SIM_DURATION/SIM_RATE;
    TestingDatabase tb = TestingDatabase(NUM_PERSONS, numTestsPerPerson);
	ChartWriter chartWriter(NUM_PERSONS);
    

    // Figure out who wears masks and who doesn’t
    int i;
    for (i = 0; i < NUM_PERSONS; i++)
    {
        Person person = Person(i, -1, -1, rand() % 2 == 0);
        people[i] = person;
        bool TrueFalse = (rand() % 100) < MASK_PROB;
        if(TrueFalse){
            person.set_mask();
            //printf("person id = %d mask = %d\n",person.bu_id, person.wears_mask);
        }
    }

    Graph g = Graph(people, CONNECTION_PROB);

    //#pragma omp barrier //wait until all threads are done

    float time = 0.0f; //current time
    while(time < SIM_DURATION){
        
        //TODO: parallelize this for loop
        int i;
        //update the states of each person for the current time frame
        //#pragma omp parallel for shared (people, g) private (i) // uncomment this line works only comment it for testing!
        for(i = 0; i < NUM_PERSONS; i++) {
            //push back this person's covid state from the previous time frame,
            //making room for their updated state in the current time frame.
            //If they have covid, this includes determining whether they have recovered or died, etc.
            people[i].update_covid_state();
            
            //determine if this person is being infected in the current time frame based on the covid states of their neighbors from the previous time frame (i.e. person.covid_state[0])
            people[i].calculate_covid(g);
            
            //update the testing database given the current state of this person (i.e. person.covid_state[1])
            bool testResult = tb.test_node(people[i].bu_id, people[i].covid_state[1]);

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


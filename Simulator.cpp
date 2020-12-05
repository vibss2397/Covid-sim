const int NUM_PERSONS = 16000;
const float SIM_DURATION = 1000.0f; //how many units of time do we run the simulation for?
const float SIM_RATE = 1.0f; //length of time between each time frame of the simulation

int main(int argc, char* argv[])
{
    // Initialize people array + graph
    Person[] people = new Person[NUM_PERSONS];
    Graph g = Graph.init(people, NUM_PERSONS);
    
    // Figure out who wears masks and who doesn’t
    // TODO: Determine if this is going to be binary (“wears mask” vs “doesn’t wear a mask”) or not
    for (int i = 0; i < NUM_PERSONS; i++)
    {
        Person person = people[i];
        float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        person.wearsMask = randomValue < Person.maskPercentageForGroup(person.type);
    }
    
    float time = 0.0f;
    
    //Main loop
    while(time < SIM_DURATION){
        // TODO: Update world state for each node with a parallelized loop
        // for each time frame, have every node calculate its updated state based on the graph from the previous time frame (each node calculates its current state by querying the Testing Database?)

        // TODO: Handle output for each time frame?
        
        time += SIM_RATE;
    }
    
    // TODO: Handle final output
}

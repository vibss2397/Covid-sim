#ifndef PERSON_H_
#define PERSON_H_

#include <vector>
#include <unordered_map>

class Graph;

class Person{
public:
    int16_t bu_id;
    int16_t age;
    // 0: safe(potential), 1 : infected, 2 : symptomatic, 3 : quarantine, 4: recovery, -1 : dead
    int16_t covid_state[2]; 
    // 0 : off, 1 : on
    int16_t wears_mask; 
    // 0 : undergrad, 1: grad, 2 : faculty, 3 : other staff
    int16_t university_status; 
    int16_t covid_counter;
    int16_t covid_mu_factor;
    Person(int16_t id = -1, int16_t uni_Status = -1, int16_t age_arg = -1, int16_t cov_state = 0, int16_t mask_state = 0);
    int16_t intialize_uni_status(int16_t e);
    int16_t initialize_age(int16_t b);
    void set_mask();
    void set_state(int16_t temp);
    int16_t set_covid_mu();
    std::unordered_map<std::string, int16_t> get_details();
    void update_covid_state();
    void calculate_covid(Graph graph_instance);

};

#endif

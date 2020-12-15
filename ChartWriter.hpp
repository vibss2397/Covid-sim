#ifndef CHARTWRITER_H_
#define CHARTWRITER_H_

#include <stdio.h>
#include <vector>
#include <fstream>
#include "Person.hpp"
#include "TestingDatabase.h"

// Class that handles collecting information from the testing database / person instances and writing them to line graphs / stacked bar graphs
// For an additional interface to view the output images, see ChartViewer.html
class ChartWriter
{
	
	// Files used to store data
	std::ofstream covidStatusByGroupFiles[5];
	std::ofstream covidTestsByGroupFiles[5];

	// Keep track of test index for each person in the testing database, so I don't need to worry about testing frequency etc,
	// I can just check to see if the index changes to determine if a person took a test since the last time the chart data
	// was updated
	int *lastKnownTestIndex;

    public:

        // Constants
		const int CHART_WIDTH = 1200;
		const int CHART_HEIGHT = 600;
		// Note: there needs to be one more entry then the number of university statuses here, the actual value will be unused
		const std::string CHART_UNIVERISTY_STATUS_NICE_NAMES[5] = {
			std::string("Undergraduate Students"),
			std::string("Graduate Students"),
			std::string("Faculty"),
			std::string("Staff"),
			std::string("")
		};
        const std::string CHART_UNIVERSITY_STATUS_SHORT_NAMES[6] = {
            std::string("undergrads"),
            std::string("grads"),
            std::string("faculty"),
            std::string("staff"),
            std::string("all-combined"),
            std::string("all-separated")
        };

        // Metrics to calculate, stored by university_status, where the last index stores stats on everyone
        int healthyCounts[5];
        int infectedCounts[5];
        int recoveredCounts[5];
        int deadCounts[5];
        int positiveTestCounts[5];
        int negativeTestCounts[5];
    
        ChartWriter(int numPeople);

        void cleanup();

        void openWebpage();

        void updateFromData(float time, std::vector<Person*> people, int num_people, TestingDatabase testingDatabase);

        void writeAllCharts();

        void writeStatusGraph(std::string filename, std::ofstream *file, int groupNumber, std::string groupName);

        void writeTestsGraph(std::string filename, std::ofstream *file, int groupNumber, std::string groupName);
    
};

#endif
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include "Person.hpp"
#include "TestingDatabase.h"
#include "ChartWriter.hpp"


ChartWriter::ChartWriter(int numPeople)
{
	for (int i = 0; i < 5; i++)
	{
		covidStatusByGroupFiles[i].open("covid-status-results-group-" + std::to_string(i) + ".dat");
		covidTestsByGroupFiles[i].open("covid-tests-results-group-" + std::to_string(i) + ".dat");
	}
	lastKnownTestIndex = new int[numPeople];
	for (int i = 0; i < numPeople; i++)
	{
		lastKnownTestIndex[i] = 0;
	}
}

// Remove generated files, and close open output streams
void ChartWriter::cleanup()
{
	for (int i = 0; i < 5; i++)
	{
		covidStatusByGroupFiles[i].close();
		covidTestsByGroupFiles[i].close();
	}
	for (int i = 0; i < 5; i++)
	{
		remove(("covid-status-results-group-" + std::to_string(i) + ".dat").c_str());
		remove(("covid-tests-results-group-" + std::to_string(i) + ".dat").c_str());
	}
}

// Attempt to open the HTML viewer in the user's default browser
void ChartWriter::openWebpage()
{
	#ifdef _WIN32
	system("start ChartViewer.html");
	#else
	system("xdg-open ChartViewer.html");
	#endif
}

// Update the data files from a given timestamp, people array, and testing database
void ChartWriter::updateFromData(float time, std::vector<Person> people, int num_people, TestingDatabase testingDatabase)
{
	// Write new output to the .dat files used in graph creation
	// Reset all counters to 0
	for (int i = 0; i < 5; i++)
	{
		healthyCounts[i] = 0;
		infectedCounts[i] = 0;
		recoveredCounts[i] = 0;
		deadCounts[i] = 0;
		positiveTestCounts[i] = 0;
		negativeTestCounts[i] = 0;
	}
	for (int i = 0; i < num_people; i++)
	{
		Person person = people[i];
		// Collect data for covid state graph(s)
		int16_t state = person.covid_state[0];
		if (state == 0)
		{
			healthyCounts[4]++;
			healthyCounts[person.university_status]++;
		}
		else if (state == 1 || state == 2 ||  state == 3)
		{
			infectedCounts[4]++;
			infectedCounts[person.university_status]++;
		}
		else if (state == 4)
		{
			recoveredCounts[4]++;
			recoveredCounts[person.university_status]++;
		}
		else if (state == -1)
		{
			deadCounts[4]++;
			deadCounts[person.university_status]++;
		}
		// Collect data for covid tests graph(s)
		int nextTestIndex = testingDatabase.test_num[i];
		if (nextTestIndex > lastKnownTestIndex[i])
		{
			if (testingDatabase.results[i][nextTestIndex-1])
			{
				positiveTestCounts[4]++;
				positiveTestCounts[person.university_status]++;
			}
			else
			{
				negativeTestCounts[4]++;
				negativeTestCounts[person.university_status]++;
			}
			lastKnownTestIndex[i] = nextTestIndex;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		covidStatusByGroupFiles[i] << time << "\t" << healthyCounts[i] << "\t" << infectedCounts[i] << "\t" << deadCounts[i] << "\t" << recoveredCounts[i] << "\n";
		covidTestsByGroupFiles[i] << time << "\t" << positiveTestCounts[i] << "\t" << negativeTestCounts[i] << "\n";
	}
}

// Generate image files from the intermediate data files
void ChartWriter::writeAllCharts()
{
	// Generate group graphs using gnuplot
	for (int i = 0; i < 5; i++)
	{
		writeStatusGraph("covid-status-results-group-" + std::to_string(i) + ".dat", &covidStatusByGroupFiles[i], i, CHART_UNIVERISTY_STATUS_NICE_NAMES[i]);
		writeTestsGraph("covid-tests-results-group-" + std::to_string(i) + ".dat", &covidTestsByGroupFiles[i], i, CHART_UNIVERISTY_STATUS_NICE_NAMES[i]);
	}
	// Generate all groups (separate) graphs using gnuplot
	// [Yes, these commands could be built-up programmatically, but hard-coding them is easier.]
	std::string statusGraphCommand = "gnuplot -e \""
										"set terminal pngcairo size " + std::to_string(CHART_WIDTH) + "," + std::to_string(CHART_HEIGHT) + " background rgb '#E0E0E0' truecolor;"
										"set output 'covid-status-graph-" + CHART_UNIVERSITY_STATUS_SHORT_NAMES[5] + ".png';"
										"set title 'Covid-19 Status Over Time';"
										"set xlabel 'Days since simulation started';"
										"set ylabel 'Number of people';"
										"set ticscale 0; set grid ytics;"
										"set key outside; set key bottom;"
										"plot 'covid-status-results-group-0.dat' using 1:2 title 'Healthy (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[0] + ")' with lines linecolor rgb '#267F00' lw 2,"
											"'covid-status-results-group-1.dat' using 1:2 title 'Healthy (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[1] + ")' with lines linecolor rgb '#4CFF00' lw 2,"
											"'covid-status-results-group-2.dat' using 1:2 title 'Healthy (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[2] + ")' with lines linecolor rgb '#B6FF00' lw 2,"
											"'covid-status-results-group-3.dat' using 1:2 title 'Healthy (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[3] + ")' with lines linecolor rgb '#42FFAA' lw 2,"
											"'covid-status-results-group-0.dat' using 1:3 title 'Infected (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[0] + ")' with lines linecolor rgb '#7F3300' lw 2,"
											"'covid-status-results-group-1.dat' using 1:3 title 'Infected (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[1] + ")' with lines linecolor rgb '#7C0649' lw 2,"
											"'covid-status-results-group-2.dat' using 1:3 title 'Infected (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[2] + ")' with lines linecolor rgb '#FF0000' lw 2,"
											"'covid-status-results-group-3.dat' using 1:3 title 'Infected (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[3] + ")' with lines linecolor rgb '#FF7A38' lw 2,"
											"'covid-status-results-group-0.dat' using 1:4 title 'Recovered (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[0] + ")' with lines linecolor rgb '#0026FF' lw 2,"
											"'covid-status-results-group-1.dat' using 1:4 title 'Recovered (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[1] + ")' with lines linecolor rgb '#0094FF' lw 2,"
											"'covid-status-results-group-2.dat' using 1:4 title 'Recovered (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[2] + ")' with lines linecolor rgb '#00E5FF' lw 2,"
											"'covid-status-results-group-3.dat' using 1:4 title 'Recovered (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[3] + ")' with lines linecolor rgb '#2E00AD' lw 2,"
											"'covid-status-results-group-0.dat' using 1:5 title 'Dead (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[0] + ")' with lines linecolor rgb '#000000' lw 2,"
											"'covid-status-results-group-1.dat' using 1:5 title 'Dead (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[1] + ")' with lines linecolor rgb '#898989' lw 2,"
											"'covid-status-results-group-2.dat' using 1:5 title 'Dead (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[2] + ")' with lines linecolor rgb '#4C4C4C' lw 2,"
											"'covid-status-results-group-3.dat' using 1:5 title 'Dead (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[3] + ")' with lines linecolor rgb '#707070' lw 2;"
									"\"";
	system(statusGraphCommand.c_str());
	std::string testsGraphCommand = "gnuplot -e \""
										"set terminal pngcairo size " + std::to_string(CHART_WIDTH) + "," + std::to_string(CHART_HEIGHT) + " background rgb '#E0E0E0' truecolor;"
										"set output 'covid-tests-graph-" + CHART_UNIVERSITY_STATUS_SHORT_NAMES[5] + ".png';"
										"set title 'Covid-19 Test Results Over Time';"
										"set xlabel 'Days since simulation started';"
										"set ylabel 'Number of tests';"
										"set style data histograms;"
										"set style histogram rowstacked;"
										"set boxwidth 1 relative;"
										"set key outside;"
										"set key bottom;"
										"set style fill solid 1.0 border -1;"
										"plot 'covid-tests-results-group-0.dat' using 3 title 'Negative test results (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[0] + ")' linecolor rgb '#267F00',"
											"'covid-tests-results-group-1.dat' using 3 title 'Negative test results (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[1] + ")' linecolor rgb '#4CFF00',"
											"'covid-tests-results-group-2.dat' using 3 title 'Negative test results (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[2] + ")' linecolor rgb '#B6FF00',"
											"'covid-tests-results-group-3.dat' using 3 title 'Negative test results (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[3] + ")' linecolor rgb '#42FFAA',"
											"'covid-tests-results-group-0.dat' using 2 title 'Positive test results (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[0] + ")' linecolor rgb '#7F3300',"
											"'covid-tests-results-group-1.dat' using 2 title 'Positive test results (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[1] + ")' linecolor rgb '#7C0649',"
											"'covid-tests-results-group-2.dat' using 2 title 'Positive test results (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[2] + ")' linecolor rgb '#FF0000',"
											"'covid-tests-results-group-3.dat' using 2:xticlabels(1) title 'Positive test results (" + CHART_UNIVERISTY_STATUS_NICE_NAMES[3] + ")' linecolor rgb '#FF7A38'"
									"\"";
	system(testsGraphCommand.c_str());
}
	
// Write a single line graph of infection status counts over time
void ChartWriter::writeStatusGraph(std::string filename, std::ofstream *file, int groupNumber, std::string groupName)
{
	if (file != NULL)
		file->flush();
	std::string statusGraphCommand = "gnuplot -e \""
										"set terminal pngcairo size " + std::to_string(CHART_WIDTH) + "," + std::to_string(CHART_HEIGHT) + " background rgb '#E0E0E0' truecolor;"
										"set output 'covid-status-graph-" + CHART_UNIVERSITY_STATUS_SHORT_NAMES[groupNumber] + ".png';"
										"set title 'Covid-19 Status Over Time (" + (groupNumber > 3 ? "all groups" : (groupName + " only")) + ")';"
										"set xlabel 'Days since simulation started';"
										"set ylabel 'Number of people';"
										"set ticscale 0;"
										"set grid ytics;"
										"set key outside;"
										"set key bottom;"
										"set style line 1 linecolor rgb 'red' lw 2;"
										"set style line 2 linecolor rgb 'black' lw 2;"
										"set style line 3 linecolor rgb 'blue' lw 2;"
										"set style line 4 linecolor rgb 'green' lw 2;"
										"plot '" + filename + "' using 1:2 title 'Healthy' with lines ls 4,"
											"'' using 1:3 title 'Infected' with lines ls 1,"
											"'' using 1:5 title 'Recovered' with lines ls 3,"
											"'' using 1:4 title 'Dead' with lines ls 2"
									"\"";
	system(statusGraphCommand.c_str());
}

// Write a single stacked bar graph of test results over time
void ChartWriter::writeTestsGraph(std::string filename, std::ofstream *file, int groupNumber, std::string groupName)
{
	if (file != NULL)
		file->flush();
	std::string testsGraphCommand = "gnuplot -e \""
										"set terminal pngcairo size " + std::to_string(CHART_WIDTH) + "," + std::to_string(CHART_HEIGHT) + " background rgb '#E0E0E0' truecolor;"
										"set output 'covid-tests-graph-" + CHART_UNIVERSITY_STATUS_SHORT_NAMES[groupNumber] + ".png';"
										"set title 'Covid-19 Test Results Over Time (" + (groupNumber > 3 ? "all groups" : (groupName + " only")) + ")';"
										"set xlabel 'Days since simulation started';"
										"set ylabel 'Number of tests';"
										"set style data histograms;"
										"set style histogram rowstacked;"
										"set boxwidth 1 relative;"
										"set key outside;"
										"set key bottom;"
										"set style fill solid 1.0 border -1;"
										"plot '" + filename + "' using 3 title 'Negative test results' linecolor rgb 'green',"
											"'' using 2:xticlabels(1) title 'Positive test results' linecolor rgb 'red'"
									"\"";
	system(testsGraphCommand.c_str());
}

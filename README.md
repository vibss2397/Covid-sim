# Covid-sim

## Dependencies:
- gnuplot must be installed (and in the PATH environment variable)

## Building:

### Linux:
```bash
g++ -fopenmp Simulator.cpp Graph.cpp Person.cpp TestingDatabase.cpp Helper_Func.cpp ChartWriter.cpp -o simulator
```

### Windows:
```
g++ -fopenmp Simulator.cpp Graph.cpp Person.cpp TestingDatabase.cpp Helper_Func.cpp ChartWriter.cpp -o simulator.exe
```

## Running
```
~> ./simulator
```


## Documentation
Document


When run, statistics will be printed out each iteration, and at the end of the run gnuplot will be called to generate line / stacked bar graphs.
These graphs can be viewed through the html file ChartViewer.html, or as standalone images.


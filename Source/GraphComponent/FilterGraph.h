#ifndef __FILTERGRAPH_H_3CCF0ED1__
#define __FILTERGRAPH_H_3CCF0ED1__

//==============================================================================
class FilterGraph    : public Component,
                       public SettableTooltipClient
{
public:
    FilterGraph (int numFilters);
    ~FilterGraph();
    
    enum TraceType
    {
        Magnitude,
        Phase
    };
    
    void paint (Graphics&);
    void resized();
    
    void setNumHorizontalLines (int newValue);
    float yToHeight(float yPos);
    float xToWidth(float yPos);

    void setGraphPlot(const StringArray & data);

    void setMaxDB(const float & db);
    void setFreq(const float & lowF, const float & highF);

    void setTraceColour(Colour newColour);
    void setBgGradientColour(Colour grad1, Colour grad2);
    void setGridLineColour(Colour lineColour);

    void mouseDown(const MouseEvent &event);

    float xToFreq(float xPos);
    float freqToX(float freq);

private:
    Colour traceColour;
    Colour backgroundGrd1Colour, backgroundGrd2Colour;
    Colour gridLinesColour;
    TraceType traceType;
    float lowFreq, highFreq;
    float maxdB;
    float minPlot, maxPlot;
    int numHorizontalLines;
    Array<double> plotDoubleArray;
    Path gridPath, tracePath;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterGraph)
};
#endif  // __FILTERGRAPH_H_3CCF0ED1__

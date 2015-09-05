/*
  ==============================================================================

    FilterGraph.cpp
    Created: 21 Oct 2014 2:16:43pm
    Author:  cmodi

  ==============================================================================
*/
#include "stdafx.h"
#include "FilterGraph.h"

//==============================================================================
FilterGraph::FilterGraph (int numFiltersInit)
{    
    numHorizontalLines = 7;
    setSize (500, 300);
    traceColour = Colour (0xaa00ff00);
    backgroundGrd1Colour = Colour(0xff232338);
    backgroundGrd2Colour = Colour(0xff21222a);
    gridLinesColour = Colour(0x60ffffff);
    maxdB = 6;
    lowFreq = 20;
    highFreq = 20000;
    traceType = Magnitude;
}

FilterGraph::~FilterGraph()
{
}

void FilterGraph::paint (Graphics& g)
{
    // get size info =======================================================================================
    float width = (float) getWidth();
    float height = (float) getHeight();
    
    // paint the display background ============================================================================
    g.setGradientFill(ColourGradient(backgroundGrd1Colour, width / 2, height / 2, backgroundGrd2Colour, 2.5f, height / 2, true));
    g.fillRoundedRectangle (2.5f, 2.5f, width - 5, height - 5, 10.000f);
    
    // paint the only trace at the moment ======================================================================
    tracePath.clear();

    if (plotDoubleArray.size() > 0) {

        // draw graph now using gain values from back end
        if (traceType == Magnitude)
        {
            float xPos = xToWidth(0) + 10;
            float yPos = yToHeight(plotDoubleArray[0]);
            tracePath.startNewSubPath(xPos, yPos);
            for (int pc = 1; pc < plotDoubleArray.size(); pc++)
            {
                xPos = xToWidth(pc) + 10;
                yPos = yToHeight(plotDoubleArray[pc]);
                tracePath.lineTo(xPos, yPos);
            }
        }

        g.setColour(traceColour);
        g.strokePath(tracePath, PathStrokeType(3.0f));
    }
    
    // paint the display grid lines ===============================================================================
    g.setColour (Colour (0xaaffffff));
    String axisLabel;
    if (traceType == Magnitude)  axisLabel = String (maxdB, 1) + "dB";
    
    g.setFont (Font ("Arial Rounded MT", 12.0f, Font::plain));
    g.drawText (axisLabel, 6, (int) ((height - 5) / (numHorizontalLines + 1) -9.5f), 45, 12, Justification::left, false);
    g.drawText (String ("-") + axisLabel, 6, (int) (numHorizontalLines * (height - 5) / (numHorizontalLines + 1) + 3.5f), 45, 12, Justification::left, false);
    
    gridPath.clear();
    for (int lineNum = 1; lineNum < numHorizontalLines + 1; lineNum++)
    {
        float yPos = lineNum * (height - 5) / (numHorizontalLines + 1) + 2.5f;
        gridPath.startNewSubPath (2.5f, yPos);
        gridPath.lineTo (width - 2.5f, yPos);
    }
    
    float order = (pow (10, floor (log10 (lowFreq))));
    float rounded = order * (floor(lowFreq / order) + 1);
    for (float freq = rounded; freq < highFreq; freq += pow (10, floor (log10 (freq))))
    {
        float xPos = freqToX (freq);
        gridPath.startNewSubPath (xPos, 2.5f);
        gridPath.lineTo (xPos, height - 2.5f);
    }
    g.excludeClipRegion (juce::Rectangle <int> (6, (int) ((height - 5) / (numHorizontalLines + 1) -9.5f), 45, 12));
    g.excludeClipRegion (juce::Rectangle <int> (6, (int) (numHorizontalLines * (height - 5) / (numHorizontalLines + 1) + 3.5f), 45, 12));
    g.setColour(gridLinesColour);
    g.strokePath (gridPath, PathStrokeType (1.0f));
    
    // draw the boarder ======================================================================================
    g.setColour (Colours::black);
    g.drawRoundedRectangle (2.5f, 2.5f, width - 5, height - 5, 10.000f, 5.000f);
}

void FilterGraph::resized()
{
}

void FilterGraph::setNumHorizontalLines (int newValue)
{
    numHorizontalLines = newValue;
    repaint();
}

float FilterGraph::yToHeight(float yPos)
{
    float height = (float)getHeight()-3.5;

    // Reverse Y-axis to point upwards
    float result = height - height*(yPos - minPlot) / (maxPlot - minPlot);
    return result;
}

float FilterGraph::xToWidth(float yPos)
{
    float width = (float)getWidth()-2.5;
    float min, max;
    min = 0.0f;
    max = (float)plotDoubleArray.size();
    float result = width*(yPos - min) / (max - min);
    return result;
}

void FilterGraph::setTraceColour (Colour newColour)
{
    traceColour = newColour;
    repaint();
}

void FilterGraph::mouseDown (const MouseEvent &event)
{
 /*   Point <int> mousePos =  getMouseXYRelative();
    int xPos = mousePos.getX();
    float freq = xToFreq (xPos);*/
}

float FilterGraph::xToFreq(float xPos)
{
    float width = (float)getWidth();
    return lowFreq * pow((highFreq / lowFreq), ((xPos - 2.5f) / (width - 5.0f)));
}

float FilterGraph::freqToX(float freq)
{
    float width = (float)getWidth();
    return (width - 5) * (log(freq / lowFreq) / log(highFreq / lowFreq)) + 2.5f;
}

void FilterGraph::setGraphPlot(const StringArray & data)
{
    if (data.size())
    {
        plotDoubleArray.clear();
        
        
        minPlot = maxPlot = 0;
        for (int i = 0; i < data.size(); i++)
        {
            double val = data[i].getDoubleValue();
            plotDoubleArray.add(val);
            if (val > maxPlot)
                maxPlot = val;

            if (val < minPlot)
                minPlot = val;
        }

        //if (minPlot == maxPlot)
        {
            for (int i = 0; i < 4096; i++)
            {
                plotDoubleArray.add(-37.65 - (0.005 * i));
            }
            DefaultElementComparator<double> doubleSorter;
            plotDoubleArray.sort(doubleSorter);
            minPlot = plotDoubleArray.getFirst();
            maxPlot = plotDoubleArray.getLast();
        }
        
    }
    repaint();
}

void FilterGraph::setMaxDB(const float & db)
{
    maxdB = db;
    repaint();
}
void FilterGraph::setFreq(const float & lowF, const float & highF)
{
    lowFreq = lowF;
    highFreq = highF;
    repaint();
}

void FilterGraph::setBgGradientColour(Colour grad1, Colour grad2)
{
    backgroundGrd1Colour = grad1;
    backgroundGrd2Colour = grad2;
    repaint();
}

void FilterGraph::setGridLineColour(Colour lineColour)
{
    gridLinesColour = lineColour;
    repaint();
}
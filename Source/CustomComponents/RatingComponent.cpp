#include "stdafx.h"
#include "RatingComponent.h"

StarComponent::StarComponent(int indx)
:mousein(false),
mouseclick(false)
{
	index = indx;
	setSize(18, 18);
}
void StarComponent:: paint(Graphics& g) 
{
	Path star;
	star.addStar(Point<float>(), 5, 1.0f, 2.0f);

	if ((mousein == true || mouseclick == true))
		g.setColour(Colour(0xFFF2C714));
	else
		g.setColour(Colour(0xFFEFEFEF));

	g.fillPath(star, star.getTransformToScaleToFit(getLocalBounds().reduced(2).toFloat(), true));
}

void StarComponent::mouseDown(const MouseEvent&)
{
	sendActionMessage(((String)RatingComponent::Click + ":" + (String)index));
}

void StarComponent::mouseEnter(const MouseEvent& event)
{
	sendActionMessage(((String)RatingComponent::Enter + ":" + (String)index));
}

void StarComponent::mouseExit(const MouseEvent& event)
{
	sendActionMessage(((String)RatingComponent::Exit + ":" + (String)index));
}

void StarComponent::setmouseEvents(int mdetails)
{
	switch (mdetails)
	{
	case RatingComponent::Click:
		mouseclick = true;
			break;
	case RatingComponent::Enter:
		mousein = true;
			break;
	case RatingComponent::Exit:
		mousein = false;
		break;
	case RatingComponent::Unclick:
		mouseclick = false;
		break;
	default:
		break;
	}
}
bool StarComponent::getmouseclick()
{
	return mouseclick;
}

////////////////////////////////////////////////////////

RatingComponent::RatingComponent(int no_of_compo)
{
	lastClick = 0;

	for (int i = 0; i < no_of_compo; i++)
	{
		ratcomp.push_back(new StarComponent(i));
		addAndMakeVisible(ratcomp[i]);
		ratcomp[i]->addActionListener(this);
	}
	setSize(150, 30);
}


void RatingComponent::paint(Graphics& g)
{
	Path rect;
	rect.addRectangle(0, 0, 150, 30);
	g.setColour(Colours::black);
}
void RatingComponent::resized()
{
	int x = 0;
    std::for_each(ratcomp.begin(), ratcomp.end(), [&x](StarComponent * star){
        star->setBounds(x, 0, 20, 20);
        x = x + 20;
	});
}

void RatingComponent:: actionListenerCallback(const String & 	message)
{
	int index = message.fromFirstOccurrenceOf(":", false, false).getIntValue();
	MouseDetails md = (MouseDetails)message.upToFirstOccurrenceOf(":", false, false).getIntValue();
	
	switch (md)
	{
	case RatingComponent::Click:
			if (ratcomp[index]->getmouseclick() && index == lastClick)
			{
			  for (int i = 0; i < ratcomp.size(); i++)
			   {
				 ratcomp[i]->setmouseEvents(Unclick);
				 ratcomp[i]->repaint();
			   }
			break;
		    }
			for (int i = 0; i < ratcomp.size(); i++)
			{
				if (i <= index)
					ratcomp[i]->setmouseEvents(Click);
				else
					ratcomp[i]->setmouseEvents (Unclick);
				ratcomp[i]->repaint();
			}
		lastClick = index;
		break;
	case RatingComponent::Enter:
		for (int i = 0; i < ratcomp.size(); i++)
		{
			if (i <= index)
				ratcomp[i]->setmouseEvents(Enter);
			else
				ratcomp[i]->setmouseEvents(Exit);
			ratcomp[i]->repaint();
		}
		break;
	case RatingComponent::Exit:

		for (int i = 0; i < ratcomp.size(); i++)
		{
			ratcomp[i]->setmouseEvents(Exit);
			ratcomp[i]->repaint();
		}
		break;
	default:
		break;
	}

}
int RatingComponent::getratting()
{
	return lastClick+1;
}
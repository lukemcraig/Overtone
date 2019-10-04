#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OvertoneFilterEditor::OvertoneFilterEditor(OvertoneFilterAudioProcessor& p,
                                           ParameterHelper& ph,
                                           MidiKeyboardState& ks,
                                           LevelMeterAudioSource& inputLevel,
                                           LevelMeterAudioSource& wetMixLevel,
                                           LevelMeterAudioSource& outputLevel)
    : AudioProcessorEditor(&p),
      processor(p),
      parameterHelper(ph),
      keyboardState(ks),
      keyboard(p, ks, MidiKeyboardComponent::horizontalKeyboard),
      inputMeter(inputLevel),
      wetMixMeter(wetMixLevel),
      outputMeter(outputLevel)
{
    {
        addAndMakeVisible(standardSlider);
        standardAttachment.reset(new SliderAttachment(parameterHelper.valueTreeState,
                                                      parameterHelper.PID_PITCH_STANDARD, standardSlider));

        standardLabel.setText("Pitch Standard", dontSendNotification);
        standardLabel.attachToComponent(&standardSlider, true);
        addAndMakeVisible(standardLabel);
    }
    {
        addAndMakeVisible(qSlider);
        qAttachment.reset(new SliderAttachment(parameterHelper.valueTreeState, parameterHelper.PID_Q, qSlider));

        qLabel.setText("Q", dontSendNotification);
        qLabel.attachToComponent(&qSlider, true);
        addAndMakeVisible(qLabel);
    }
    {
        addAndMakeVisible(gainSlider);
        gainAttachment.reset(new SliderAttachment(parameterHelper.valueTreeState, parameterHelper.PID_GAIN,
                                                  gainSlider));

        gainLabel.setText("Gain", dontSendNotification);
        gainLabel.attachToComponent(&gainSlider, true);
        addAndMakeVisible(gainLabel);
    }
    {
        addAndMakeVisible(wetDrySlider);
        wetDryAttachment.reset(new SliderAttachment(parameterHelper.valueTreeState, parameterHelper.PID_WETDRY,
                                                    wetDrySlider));

        wetDryLabel.setText("Wet/Dry", dontSendNotification);
        wetDryLabel.attachToComponent(&wetDrySlider, true);
        addAndMakeVisible(wetDryLabel);
    }
    {
        borderPath.setFill(Colours::transparentBlack);
        borderPath.setStrokeType(PathStrokeType(1));
        borderPath.setStrokeFill(Colours::white);
        addAndMakeVisible(borderPath);
    }
    {
        nameLabel.setText("Overtone Filter - Luke M. Craig - " __DATE__ + String(" ") + __TIME__, dontSendNotification);
        addAndMakeVisible(nameLabel);
    }
    {
        addAndMakeVisible(inputMeter);
        addAndMakeVisible(wetMixMeter);
        addAndMakeVisible(outputMeter);
    }
    addAndMakeVisible(keyboard);
    setResizable(true, true);
    setResizeLimits(400, 400, 1680, 1050);
    setSize(800, 400);
    // ----
}

OvertoneFilterEditor::~OvertoneFilterEditor()
{
}

//==============================================================================
void OvertoneFilterEditor::paint(Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    g.setColour(Colours::white);
    g.setFont(15.0f);
}

void OvertoneFilterEditor::resized()
{
    //TODO clean all this up
    {
        const auto bounds = getBounds().toFloat();
        RectanglePlacement placement(RectanglePlacement::fillDestination);
        const auto w = bgPath.getDrawableBounds().proportionOfWidth(0.17f);
        const auto fitTransform = placement.getTransformToFit(bgPath.getDrawableBounds().reduced(w), bounds);
        bgPath.setTransform(fitTransform.followedBy(AffineTransform::translation(0, 110.0f)));
    }

    auto area = getLocalBounds();
    // margins
    area.reduce(10, 10);

    {
        nameLabel.setJustificationType(Justification::centred);
        auto nameArea = area;
        nameLabel.setPaintingIsUnclipped(true);
        nameLabel.setBounds(nameArea.removeFromTop(5));
    }
    {
        auto pad = 10;
        auto w = -pad + (area.getWidth() - nameLabel.getFont().getStringWidthFloat(nameLabel.getText())) / 2.0f;
        Path path;
        path.startNewSubPath(area.getX() + w, area.getY());
        const auto topLeft = area.getTopLeft();
        path.lineTo(topLeft.getX(), topLeft.getY());
        const auto bottomLeft = area.getBottomLeft();
        path.lineTo(bottomLeft.getX(), bottomLeft.getY());
        const auto bottomRight = area.getBottomRight();
        path.lineTo(bottomRight.getX(), bottomRight.getY());
        const auto topRight = area.getTopRight();
        path.lineTo(topRight.getX(), topRight.getY());
        path.lineTo(topRight.getX() - w, topRight.getY());
        auto roundPath = path.createPathWithRoundedCorners(3);
        borderPath.setPath(roundPath);
    }
    area.reduce(10, 10);
    auto nPanes = 2;

    const auto paneAreaHeight = area.getHeight() / nPanes;

    outputMeter.setBounds(area.removeFromRight(60));
    wetMixMeter.setBounds(area.removeFromRight(60));
    inputMeter.setBounds(area.removeFromRight(60));

    standardSlider.setBounds(area.removeFromTop(60).reduced(100, 0));
    qSlider.setBounds(area.removeFromTop(60).reduced(100, 0));
    gainSlider.setBounds(area.removeFromTop(60).reduced(100, 0));
    wetDrySlider.setBounds(area.removeFromTop(60).reduced(100, 0));

    const auto keyboardArea = area.removeFromTop(paneAreaHeight).reduced(10, 10);
    keyboard.setBounds(keyboardArea);
}

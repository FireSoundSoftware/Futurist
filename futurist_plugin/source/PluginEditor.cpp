namespace tremolo {
PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p),
      waveformAttachment{p.getParameterRefs().waveform, waveformComboBox},
      reverbAttachment{p.getParameterRefs().reverb, reverbSlider},
      presenceAttachment{p.getParameterRefs().presence, presenceSlider},
      bassAttachment{p.getParameterRefs().bass, bassSlider},
      middleAttachment{p.getParameterRefs().middle, middleSlider},
      trebleAttachment{p.getParameterRefs().treble, trebleSlider},
      masterAttachment{p.getParameterRefs().master, masterSlider},
      preampAttachment{p.getParameterRefs().preamp, preampSlider},

      //old realization
      mousePosLabel("MousePosLabel", "Mouse: 0, 0"), // <-- добавь это
      rateAttachment{p.getParameterRefs().rate, rateSlider},
      volumeAttachment{p.getParameterRefs().volume, volumeSlider},
      bypassAttachment{p.getParameterRefs().bypassed, bypassButton},
      lfoVisualizer{
          [&p](juce::AudioBuffer<float>& b) { p.readAllLfoSamples(b); },
          [&p] { return p.getSampleRateThreadSafe(); },
          [&p] { return p.getParameterRefs().bypassed.get(); }},
      about{*this, logo,
            JucePlugin_Manufacturer "\n" JucePlugin_Name "\n" __DATE__
                                    "\n" __TIME__
                                    "\nv" JucePlugin_VersionString} {
  background.setImage(juce::ImageCache::getFromMemory(
      futurist::assets::Background_png, futurist::assets::Background_pngSize));
  addAndMakeVisible(background);

  logo.setImage(
      juce::ImageCache::getFromMemory(futurist::assets::Logo_png, futurist::assets::Logo_pngSize));
  addAndMakeVisible(logo);

  const auto sideFontColor = juce::Colour{0xFF6EA0C7};
  setInterceptsMouseClicks(true, true);
  waveformLabel.setJustificationType(juce::Justification::left);
  waveformLabel.setMinimumHorizontalScale(1.f);
  waveformLabel.setFont(lookAndFeel.getSideLabelsFont());
  waveformLabel.setColour(juce::Label::textColourId, sideFontColor);
  addAndMakeVisible(waveformLabel);

  waveformComboBox.addItemList(p.getParameterRefs().waveform.choices, 1);
  waveformAttachment.sendInitialUpdate();
  addAndMakeVisible(waveformComboBox);

//rateSlider
  rateSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  rateSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox,
                        true, 0, 0);
  rateSlider.setTextValueSuffix(" Hz");
  rateSlider.setPopupDisplayEnabled(true, true, this);
  addAndMakeVisible(rateSlider);
  //rateLabel
  rateLabel.setJustificationType(juce::Justification::centred);
  rateLabel.setInterceptsMouseClicks(false, false);
  rateLabel.setFont(lookAndFeel.getRateLabelFont());
  addAndMakeVisible(rateLabel);

  //reverbSlider
  reverbSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  reverbSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox,
                             true, 0, 0);
  reverbSlider.setTextValueSuffix(" Hz");
  reverbSlider.setPopupDisplayEnabled(true, true, this);
  addAndMakeVisible(reverbSlider);
  //reverbLabel
  reverbLabel.setJustificationType(juce::Justification::centred);
  reverbLabel.setInterceptsMouseClicks(false, false);
  reverbLabel.setFont(lookAndFeel.getRateLabelFont());
  addAndMakeVisible(reverbLabel);

  //presenceSlider
  presenceSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  presenceSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox,
                             true, 0, 0);
  presenceSlider.setTextValueSuffix(" Hz");
  presenceSlider.setPopupDisplayEnabled(true, true, this);
  addAndMakeVisible(presenceSlider);
  //presenceLabel
  presenceLabel.setJustificationType(juce::Justification::centred);
  presenceLabel.setInterceptsMouseClicks(false, false);
  presenceLabel.setFont(lookAndFeel.getRateLabelFont());
  addAndMakeVisible(presenceLabel);

  //bassSlider
  bassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  bassSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox,
                             true, 0, 0);
  bassSlider.setTextValueSuffix(" Hz");
  bassSlider.setPopupDisplayEnabled(true, true, this);
  addAndMakeVisible(bassSlider);
  //bassLabel
  bassLabel.setJustificationType(juce::Justification::centred);
  bassLabel.setInterceptsMouseClicks(false, false);
  bassLabel.setFont(lookAndFeel.getRateLabelFont());
  addAndMakeVisible(bassLabel);

  //middleSlider
  middleSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  middleSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox,
                             true, 0, 0);
  middleSlider.setTextValueSuffix(" Hz");
  middleSlider.setPopupDisplayEnabled(true, true, this);
  addAndMakeVisible(middleSlider);
  //middleLabel
  middleLabel.setJustificationType(juce::Justification::centred);
  middleLabel.setInterceptsMouseClicks(false, false);
  middleLabel.setFont(lookAndFeel.getRateLabelFont());
  addAndMakeVisible(middleLabel);

  //trebleSlider
  trebleSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  trebleSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox,
                             true, 0, 0);
  trebleSlider.setTextValueSuffix(" Hz");
  trebleSlider.setPopupDisplayEnabled(true, true, this);
  addAndMakeVisible(trebleSlider);
  //trebleLabel
  trebleLabel.setJustificationType(juce::Justification::centred);
  trebleLabel.setInterceptsMouseClicks(false, false);
  trebleLabel.setFont(lookAndFeel.getRateLabelFont());
  addAndMakeVisible(trebleLabel);

  //masterSlider
  masterSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  masterSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox,
                             true, 0, 0);
  masterSlider.setTextValueSuffix(" Hz");
  masterSlider.setPopupDisplayEnabled(true, true, this);
  addAndMakeVisible(masterSlider);
  //masterLabel
  masterLabel.setJustificationType(juce::Justification::centred);
  masterLabel.setInterceptsMouseClicks(false, false);
  masterLabel.setFont(lookAndFeel.getRateLabelFont());
  addAndMakeVisible(masterLabel);


  //preampSlider
  preampSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  preampSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox,
                             true, 0, 0);
  preampSlider.setTextValueSuffix(" Hz");
  preampSlider.setPopupDisplayEnabled(true, true, this);
  addAndMakeVisible(preampSlider);
  //preampLabel
  preampLabel.setJustificationType(juce::Justification::centred);
  preampLabel.setInterceptsMouseClicks(false, false);
  preampLabel.setFont(lookAndFeel.getRateLabelFont());
  addAndMakeVisible(preampLabel);




  //volumeSlider
  volumeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  volumeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox,
                           true, 0, 0);
  volumeSlider.setTextValueSuffix(" times");
  volumeSlider.setPopupDisplayEnabled(true, true, this);
  addAndMakeVisible(volumeSlider);
  //volumeLabel
  volumeLabel.setJustificationType(juce::Justification::centred);
  volumeLabel.setInterceptsMouseClicks(false, false);
  volumeLabel.setFont(lookAndFeel.getRateLabelFont());
  addAndMakeVisible(volumeLabel);


  bypassLabel.setJustificationType(juce::Justification::left);
  bypassLabel.setMinimumHorizontalScale(1.f);
  bypassLabel.setFont(lookAndFeel.getSideLabelsFont());
  bypassLabel.setColour(juce::Label::textColourId, sideFontColor);
  addAndMakeVisible(bypassLabel);

  bypassButton.onClick = [this]() {
    bypassButton.setButtonText(bypassButton.getToggleState() ? "Bypassed"
                                                             : "Off");
  };
  bypassButton.onClick();
  addAndMakeVisible(bypassButton);

  lfoVisualizer.setCurveWidth(2.f);
  lfoVisualizer.setCurveColor(
      lookAndFeel.getColor(CustomLookAndFeel::Colors::orange));
  lfoVisualizer.setBackgroundColor(juce::Colours::transparentBlack);
  addAndMakeVisible(lfoVisualizer);

  setLookAndFeel(&lookAndFeel);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.

  mousePosLabel.setJustificationType(juce::Justification::topRight);
  mousePosLabel.setFont(juce::Font{12.0f});
  mousePosLabel.setColour(juce::Label::textColourId, juce::Colours::white);
  mousePosLabel.setInterceptsMouseClicks(false, false);
  addAndMakeVisible(mousePosLabel);

  setSize(WIN_WIDTH, WIN_HEIGHT);
}

PluginEditor::~PluginEditor() {
  setLookAndFeel(nullptr);
}

void PluginEditor::resized() {
  const auto bounds = getLocalBounds();

  background.setBounds(bounds);

  logo.setBounds({16, 16, 105, 24});

  //SLIDER
  //rateSlider
  int x = 490;
  int delta = 74;
  rateSlider.setBounds(290, 475, 50, 50);
  rateLabel.setBounds(290, 475, 50, 50);
  //reverbSlider
  reverbSlider.setBounds(390, 475, 50, 50);
  reverbLabel.setBounds(390, 475, 50, 50);
    //presenceSlider
  presenceSlider.setBounds(x, 475, 50, 50);
  presenceLabel.setBounds(x, 475, 50, 50);
      //bassSlider
  bassSlider.setBounds(x+delta, 475, 50, 50);
  bassLabel.setBounds(x+delta, 475, 50, 50);
    //middleSlider
  middleSlider.setBounds(x+2*delta, 475, 50, 50);
  middleLabel.setBounds(x+2*delta, 475, 50, 50);
    //trebleSlider
  trebleSlider.setBounds(x+3*delta, 475, 50, 50);
  trebleLabel.setBounds(x+3*delta, 475, 50, 50);
    //masterSlider
  masterSlider.setBounds(x+4*delta, 475, 50, 50);
  masterLabel.setBounds(x+4*delta, 475, 50, 50);
    //preampSlider
  preampSlider.setBounds(x+5*delta, 475, 50, 50);
  preampLabel.setBounds(x+5*delta, 475, 50, 50);


  //volumeSlider
  volumeSlider.setBounds(x+6*delta, 475, 50, 50);
  volumeLabel.setBounds(x+6*delta, 475, 50, 50);







  mousePosLabel.setBounds(bounds.reduced(10).removeFromTop(24));  // 10px от 
}

void PluginEditor::mouseMove(const juce::MouseEvent& e)
{
    mousePosLabel.setText(
        juce::String("Mouse: ") + juce::String(e.x) + ", " + juce::String(e.y),
        juce::dontSendNotification
    );
}
}  // namespace tremolo

#pragma once

namespace tremolo {
class PluginEditor : public juce::AudioProcessorEditor {
public:
  explicit PluginEditor(PluginProcessor&);
  ~PluginEditor() override;

  void resized() override;
  void mouseMove (const juce::MouseEvent& e) override; 

private:
  juce::ImageComponent background;
  juce::ImageComponent logo;

  juce::Label waveformLabel{"waveform label", "WAVEFORM"};
  juce::ComboBox waveformComboBox;
  juce::ComboBoxParameterAttachment waveformAttachment;
  //reverb
  juce::Label reverbLabel{"reverb label", "Reverb"};
  juce::Slider reverbSlider;
  juce::SliderParameterAttachment reverbAttachment;
  //presence
  juce::Label presenceLabel{"presence label", "Presence"};
  juce::Slider presenceSlider;
  juce::SliderParameterAttachment presenceAttachment;
  //bass
  juce::Label bassLabel{"bass label", "Bass"};
  juce::Slider bassSlider;
  juce::SliderParameterAttachment bassAttachment;
  //middle
  juce::Label middleLabel{"middle label", "Middle"};
  juce::Slider middleSlider;
  juce::SliderParameterAttachment middleAttachment;
  //treble
  juce::Label trebleLabel{"treble label", "Treble"};
  juce::Slider trebleSlider;
  juce::SliderParameterAttachment trebleAttachment;
  //master
  juce::Label masterLabel{"master label", "Master"};
  juce::Slider masterSlider;
  juce::SliderParameterAttachment masterAttachment;
  //pre_amp
  juce::Label preampLabel{"preamp label", "Pre-amp"};
  juce::Slider preampSlider;
  juce::SliderParameterAttachment preampAttachment;

  //old realization
  //rate
  juce::Label rateLabel{"rate label", "RATE"};
  juce::Slider rateSlider;
  juce::SliderParameterAttachment rateAttachment;
  //reverb
  //volume
  juce::Label volumeLabel{"volume label", "VOLUME"};
  juce::Slider volumeSlider;
  juce::SliderParameterAttachment volumeAttachment;


  juce::Label bypassLabel{"bypass label", "BYPASS"};
  juce::ToggleButton bypassButton{"BYPASSED"};
  juce::ButtonParameterAttachment bypassAttachment;

  LfoVisualizer lfoVisualizer;
  MessageOnClick about;

  CustomLookAndFeel lookAndFeel;
  juce::Label mousePosLabel;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
}  // namespace tremolo

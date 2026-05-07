#pragma once

namespace tremolo {
struct Parameters {
  explicit Parameters(juce::AudioProcessor&);
  juce::AudioParameterFloat& reverb;
  juce::AudioParameterFloat& presence;
  juce::AudioParameterFloat& bass;
  juce::AudioParameterFloat& middle;
  juce::AudioParameterFloat& treble;
  juce::AudioParameterFloat& master;
  juce::AudioParameterFloat& preamp;

  //old realization
  juce::AudioParameterFloat& rate;
  juce::AudioParameterFloat& volume;
  juce::AudioParameterBool& bypassed;
  juce::AudioParameterChoice& waveform;

  JUCE_DECLARE_NON_COPYABLE(Parameters)
  JUCE_DECLARE_NON_MOVEABLE(Parameters)
};
}  // namespace tremolo

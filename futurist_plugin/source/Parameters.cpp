
namespace tremolo {
namespace {
auto& addParameterToProcessor(juce::AudioProcessor& processor, auto parameter) {
  auto& result = *parameter;
  processor.addParameter(parameter.release());
  return result;
}


juce::AudioParameterFloat& createVolumeParameter(
    juce::AudioProcessor& processor) {
    constexpr auto versionHint = 1;
    return addParameterToProcessor(
    processor,
    std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"modulation.volume", versionHint}, "Volume",
        juce::NormalisableRange<float>{0.0f, 10.0f, 0.1f, 0.4f}, 5.f,
        juce::AudioParameterFloatAttributes{}.withLabel("times")));
}


juce::AudioParameterFloat& createModulationRateParameter(
    juce::AudioProcessor& processor) {
  constexpr auto versionHint = 1;
  return addParameterToProcessor(
      processor,
      std::make_unique<juce::AudioParameterFloat>(
          juce::ParameterID{"modulation.rate", versionHint}, "Modulation rate",
          juce::NormalisableRange<float>{0.1f, 440.f, 0.01f, 0.4f}, 5.f,
          juce::AudioParameterFloatAttributes{}.withLabel("Hz")));
}

juce::AudioParameterBool& createBypassedParameter(
    juce::AudioProcessor& processor) {
  constexpr auto versionHint = 1;
  return addParameterToProcessor(
      processor,
      std::make_unique<juce::AudioParameterBool>(
          juce::ParameterID{"bypassed", versionHint}, "Bypass", false));
}

juce::AudioParameterChoice& createWaveformParameter(
    juce::AudioProcessor& processor) {
  constexpr auto versionHint = 1;
  return addParameterToProcessor(
      processor,
      std::make_unique<juce::AudioParameterChoice>(
          juce::ParameterID{"modulation.waveform", versionHint},
          "Modulation waveform", juce::StringArray{"Sine", "Triangle"}, 0));
}
}  // namespace

Parameters::Parameters(juce::AudioProcessor& processor)
    :
      reverb{createModulationRateParameter(processor)},
      presence{createModulationRateParameter(processor)},
      bass{createModulationRateParameter(processor)},
      middle{createModulationRateParameter(processor)},
      treble{createModulationRateParameter(processor)},
      master{createModulationRateParameter(processor)},
      preamp{createModulationRateParameter(processor)},



      //old realization
      rate{createModulationRateParameter(processor)},
      volume{createVolumeParameter(processor)},
      bypassed{createBypassedParameter(processor)},
      waveform{createWaveformParameter(processor)} {}
}  // namespace tremolo

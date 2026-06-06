  /*
Creator: Tony Do
Copyright: vanhuong.do@asicland.com
Date: 2023.05.12
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string>
#include <iostream>
// #include <include/Tremolo/CSharedMemory.h>



namespace tremolo {

PluginProcessor::PluginProcessor()
    : AudioProcessor(
          BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , _shm(std::make_unique<CSharedMemroy>()){

    _shm_thread = std::thread(&PluginProcessor::worker_shm, this);

}
void PluginProcessor::worker_shm() {
  _shm_thread_running = true;
  int id_read = 1234;
  _shm->setKey(id_read);
  while (_shm_thread_running) {
    std::this_thread::sleep_for(std::chrono::milliseconds(40));
    _shm->readDataSharedMemory(id_read, _param_list);
  }
}

const juce::String PluginProcessor::getName() const {
  return PLUGIN_NAME;
}

bool PluginProcessor::acceptsMidi() const {
  return false;
}

bool PluginProcessor::producesMidi() const {
  return false;
}

bool PluginProcessor::isMidiEffect() const {
  return false;
}

double PluginProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int PluginProcessor::getNumPrograms() {
  // Some hosts don't cope very well if you tell them there are 0 programs, so
  // this should be at least 1, even if you're not really implementing programs.
  return 1;
}

int PluginProcessor::getCurrentProgram() {
  return 0;
}

void PluginProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String PluginProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void PluginProcessor::changeProgramName(int index,
                                        const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void PluginProcessor::prepareToPlay(double sampleRate,
                                    int expectedMaxFramesPerBlock) {
  currentSampleRate = sampleRate;

  tremolo.prepare(sampleRate, expectedMaxFramesPerBlock);

  bypassTransitionSmoother.prepare(
      {.sampleRate = sampleRate,
       .maximumBlockSize = static_cast<uint32_t>(expectedMaxFramesPerBlock),
       .numChannels = static_cast<uint32_t>(juce::jmax(
           getTotalNumInputChannels(), getTotalNumOutputChannels()))});
}

void PluginProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
  tremolo.reset();
  bypassTransitionSmoother.reset();
}

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()) {
    return false;
  }

  // This checks if the input layout matches the output layout
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) {
    return false;
  }

  return true;
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                   juce::MidiBuffer& midiMessages) {
  bool gesture_control = true;


  if(false){
    // if (gesture_control) {
    //   parameters.rate = _param_list[0] * 440;
    //   tremolo.setModulationRateHz(_param_list[0] * 440);
    // }else {
    //   tremolo.setModulationRateHz(parameters.rate);
    // }
  }
  parameters.volume = _param_list[0];
  tremolo.setVolume(parameters.volume);
  tremolo.process(buffer);
}

bool PluginProcessor::hasEditor() const {
  return true;
}

// This function will be called to create an instance of the editor
juce::AudioProcessorEditor* PluginProcessor::createEditor() {
  return new PluginEditor(*this);
}

void PluginProcessor::getStateInformation(juce::MemoryBlock& destData) {
  juce::MemoryOutputStream outputStream{destData, true};
  JsonSerializer::serialize(parameters, outputStream);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes) {

}

Parameters& PluginProcessor::getParameterRefs() noexcept {
  return parameters;
}

juce::AudioProcessorParameter* PluginProcessor::getBypassParameter()
    const noexcept {
  return &parameters.bypassed;
}

void PluginProcessor::readAllLfoSamples(
  juce::AudioBuffer<float>& bufferToFill) {
  // tremolo.readAllLfoSamples(bufferToFill);
}

double PluginProcessor::getSampleRateThreadSafe() const noexcept {
  return currentSampleRate;
}

PluginProcessor::~PluginProcessor() {
  _shm_thread_running = false;
  _shm_thread.join();
}
}  // namespace tremolo

// This creates new instances of the plugin.
// This function definition must be in the global namespace.
//ENTRY POINT
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new tremolo::PluginProcessor();
}



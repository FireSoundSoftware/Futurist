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


namespace tremolo {


class CSharedMemroy
{
private:
  int m_shmid;
  key_t m_key;
  char *m_shared_memory;
  char *read_data;

public:
    ~CSharedMemroy();
  void read_data_in_mem();
  void setShmId( int key );
  int getShmId();
  void setKey( key_t key );
  void setupSharedMemory( int size );
  void attachSharedMemory();
  void copyToSharedMemroy( std::string str );
  void readDataSharedMemory(int m_shmid);
  void close();
};

CSharedMemroy::~CSharedMemroy(){
  this->close();
  std::cout << "~CSharedMemory called" << std::endl;
}

void CSharedMemroy::setShmId( int id )
{
  m_shmid = id;
}

void CSharedMemroy::setKey( key_t key )
{
  m_key = key;
}

void CSharedMemroy::setupSharedMemory( int size )
{
  // Setup shared memory, 11 is the size
  if ( ( m_shmid = shmget(m_key, size , IPC_CREAT | 0666)) < 0 )
  {
    printf("Error getting shared memory id");
    exit( 1 );
  }
}

void CSharedMemroy::attachSharedMemory()
{
  // Attached shared memory
  if ( ( m_shared_memory = (char*)(shmat( m_shmid , NULL , 0 ))) == (char *)-1)
  {
    printf("Error attaching shared memory id");
    exit(1);
  }
}

void CSharedMemroy::copyToSharedMemroy(std::string str )
{
  // copy string to shared memory
  memcpy( m_shared_memory, str.c_str() , str.size() );
  sleep(2);
}

  void CSharedMemroy::readDataSharedMemory(int key)
{
  char* data_ = (char*)shmat(shmget(key, 0, 0666), NULL, 0);
  if (data_ == (char*)-1) {
    std::cout << "No shared memory (deleted?)" << std::endl;
    return;
  }
  char data_got[1024];
  memcpy(data_got, data_, 1023);
  data_got[1023] = 0;  // \0 на всякий
  std::cout << "I got: " << data_got << std::endl;
  shmdt(data_);
}

void CSharedMemroy::close()
{
  //    sleep(3);
  // Detach and remove shared memory
  void* shmdt( void *m_shmid );
  shmctl( m_shmid , IPC_RMID, NULL );
}

PluginProcessor::PluginProcessor()
    : AudioProcessor(
          BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {

    _shm_thread = std::thread(&PluginProcessor::worker_shm, this);










}
void PluginProcessor::worker_shm() {
  _shm_thread_running = true;
  auto n = std::make_unique<CSharedMemroy>();
  while (_shm_thread_running) {
    std::this_thread::sleep_for(std::chrono::milliseconds(40));

    // cout << "\nI sent: " << cpp_send << endl;
    // const char* cpp_ = cpp_send.c_str();
    // m.setKey(777);
    // m.setupSharedMemory(12);
    // m.attachSharedMemory();
    // m.copyToSharedMemroy((char*)(cpp_));
    // m.close();

    // get the shared memory ID
    // int id_read = 1234;
    // n.setKey(id_read);
    // n.readDataSharedMemory(id_read);

    int id_read = 1234;
    n->setKey(id_read);
    n->readDataSharedMemory(id_read);
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
  // std::cout << "ProcessBlock" << std::endl;
  // juce::ignoreUnused(midiMessages);
  //
  // juce::ScopedNoDenormals noDenormals;
  // const auto totalNumInputChannels = getTotalNumInputChannels();
  // const auto totalNumOutputChannels = getTotalNumOutputChannels();
  //
  // // In case we have more outputs than inputs, this code clears any output
  // // channels that didn't contain input data, (because these aren't
  // // guaranteed to be empty - they may contain garbage).
  // // This is here to avoid people getting screaming feedback
  // // when they first compile a plugin, but obviously you don't need to keep
  // // this code if your algorithm always overwrites all the output channels.
  // for (const auto channelToClear :
  //      std::views::iota(totalNumInputChannels, totalNumOutputChannels)) {
  //   buffer.clear(channelToClear, 0, buffer.getNumSamples());
  // }

  // const auto bypassedAndNotTransitioning =
  //     parameters.bypassed.get() && !bypassTransitionSmoother.isTransitioning();
  // const auto applySmoothing =
  //     bypassedAndNotTransitioning ? ApplySmoothing::no : ApplySmoothing::yes;
  //
  // // update the parameters
  // // Skip smoothing if fully bypassed to avoid LFO waveform morphing
  // // when parameters change under bypass ON.
  // // For example, if the LFO waveform is the sine, and the user selects
  // // the triangle under bypass ON, they will see a curved triangle slope
  // // on toggling bypass OFF, which is unexpected.
  parameters.volume = 3;
  tremolo.setModulationRateHz(parameters.rate);
  tremolo.setVolume(parameters.volume);
  // tremolo.setLfoWaveform(
  //     static_cast<Tremolo::LfoWaveform>(parameters.waveform.getIndex()),
  //     applySmoothing);
  //
  // bypassTransitionSmoother.setBypass(parameters.bypassed);
  //
  // if (bypassedAndNotTransitioning) {
  //   // avoid processing if the plugin is fully bypassed
  //   return;
  // }
  //
  // bypassTransitionSmoother.setDryBuffer(buffer);
  //
  // // apply tremolo
  tremolo.process(buffer);
  //
  // bypassTransitionSmoother.mixToWetBuffer(buffer);
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
  // juce::MemoryInputStream inputStream{data, static_cast<size_t>(sizeInBytes),
  //                                     false};
  // const auto result = JsonSerializer::deserialize(inputStream, parameters);
  //
  // if (result.failed()) {
  //   // Notify the user that reading parameters failed.
  //   // Currently, we just write the error message to the standard error stream.
  //   DBG(result.getErrorMessage());
  // }
  //
  // // Skip smoothing to avoid LFO waveform morphing
  // // when loading a project or a preset.
  // // For example, the default LFO waveform is the sine. If the project or preset
  // // has the triangle selected, the user will see a curved triangle slope
  // // on load, which is unexpected.
  // bypassTransitionSmoother.setBypassForced(parameters.bypassed);
  // tremolo.setLfoWaveform(
  //     static_cast<Tremolo::LfoWaveform>(parameters.waveform.getIndex()),
  //     ApplySmoothing::no);
  // tremolo.setModulationRateHz(parameters.rate, ApplySmoothing::no);
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



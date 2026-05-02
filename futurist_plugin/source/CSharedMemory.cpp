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

void CSharedMemroy::readDataSharedMemory(int key, std::vector<float>& buf){
  uint8_t* data = (uint8_t*)shmat(shmget(key, 0, 0666), NULL, 0);
  if (data == (uint8_t*)-1) {
    std::cout << "No shared memory (deleted?)" << std::endl;
    return;
  }
  uint32_t param1_u = (uint32_t(data[0])  << 24) |
                      (uint32_t(data[1])  << 16) |
                      (uint32_t(data[2])  << 8)  |
                      (uint32_t(data[3]));

  uint32_t param2_u = (uint32_t(data[4])  << 24) |
                      (uint32_t(data[5])  << 16) |
                      (uint32_t(data[6])  << 8)  |
                      (uint32_t(data[7]));

  uint32_t param3_u = (uint32_t(data[8])  << 24) |
                      (uint32_t(data[9])  << 16) |
                      (uint32_t(data[10]) << 8)  |
                      (uint32_t(data[11]));

  uint32_t param4_u = (uint32_t(data[12]) << 24) |
                      (uint32_t(data[13]) << 16) |
                      (uint32_t(data[14]) << 8)  |
                      (uint32_t(data[15]));

  // float param1, param2, param3, param4;

  std::memcpy(&buf[0], &param1_u, sizeof(float));
  std::memcpy(&buf[1], &param2_u, sizeof(float));
  std::memcpy(&buf[2], &param3_u, sizeof(float));
  std::memcpy(&buf[3], &param4_u, sizeof(float));

  std::cout << "I gottta: " << buf[0] << " "
                         << buf[1]<< " "
                         << buf[2] << " "
                         << buf[3] << std::endl;

  shmdt(data);
}

void CSharedMemroy::close()
{
  void* shmdt( void *m_shmid );
  shmctl( m_shmid , IPC_RMID, NULL );
}
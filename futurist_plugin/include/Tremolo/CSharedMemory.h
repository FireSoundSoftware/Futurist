#pragma once

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
    void readDataSharedMemory(int m_shmid, std::vector<float>& buf);
    void close();
};
#ifndef PSEUDO_MPI
#define PSEUDO_MPI

// init MPI
void MPI_Init(int* argc, char*** argv){}
// quit MPI
void MPI_Finalize(){};

double MPI_Wtime(){
    return 0.;
}

// Get process ID
void MPI_Comm_rank(MPI_Comm, int*){}

// Get number of processes
void MPI_Comm_size(MPI_Comm, int*){}

enum MPI_Datatype{
    MPI_INT,
    MPI_DOUBLE,
    MPI_CHAR
};

enum MPI_Comm{
    MPI_COMM_WORLD
};

enum MPI_Status{
    MPI_STATUS_IGNORE
};

enum MPI_Op{
    MPI_SUM
};

const void* MPI_IN_PLACE;

// Send message
int MPI_Send(   const void *buf, int count, MPI_Datatype datatype,
                int dest, int tag, MPI_Comm comm){
    return 0;
}

// Receve message
int MPI_Recv(   const void *buf, int count, MPI_Datatype datatype,
                int source, int tag, MPI_Comm comm, 
                MPI_Status status){
    return 0;
}

int MPI_Sendrecv(const void* sendbuf, int sendcount, 
                MPI_Datatype sendtype, int dest_rank, int sendtag,
                void *recvbuf, int recvcount, MPI_Datatype recvtype,  
                int source_rank, int recvtag, MPI_Comm comm, 
                MPI_Status* status){return 0;}

// get bufsize
void MPI_Pack_size(int count, MPI_Datatype datatype, int* bufsize){}
// allocate buffer memory
void MPI_Buffer_attach(char* buffer, int bufsize){};

int MPI_Bsend(   const void *buf, int count, MPI_Datatype datatype,
                int dest, int tag, MPI_Comm comm){
    return 0;
}

int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, 
            int root, MPI_Comm comm){return 0;}


int MPI_Reduce(const void *sendbuf, void *recvbuf, int count,
                  MPI_Datatype datatype, MPI_Op op, int root, 
                  MPI_Comm comm){return 0;}

int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count,
                  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm){
                    return 0;
                  }

void MPI_Barrier(MPI_Comm mpi_comm_world){}

/// @brief 
/// @param comm_old The source comuticator
/// @param ndims Number of dimensions
/// @param dims array<int> specifying number of processes in each dim.
/// @param periods array<int> of bool indicating if dim is periodic
/// @param reorder bool flag indicating if prossess may be reordered
/// @param comm_cart A new cartesian grid communicator
/// @return 
int MPI_Cart_create(MPI_Comm comm_old, int ndims, int* dims,
                    int* periods, int reorder, MPI_Comm *comm_cart){}

int MPI_Cart_rank(  MPI_Comm comm,
                    const int coords[], int* rank){return 0;}

/// @brief 
/// @param comm_cart Cartesian Comm
/// @param rank current rank
/// @param maxdims lenth of cords
/// @param cords return
/// @return 
int MPI_Cart_coords(MPI_Comm comm_cart, int rank, int maxdims,
                    int cords[]){}

/// @brief 
/// @param nnodes 
/// @param ndims 
/// @param dims 
/// @return 
int MPI_Dims_create(int nnodes, int ndims, int *dims){}



int MPI_ORDER_C = 0;
/// @brief 
/// @param ndims Number of dimensions
/// @param sixes[] extent of the full array in each dimension
/// @param subsizes extent of the subarray in each dimension
/// @param starts starting index of the subarray
/// @param order array storage order, MPI_ORDER_C
/// @param oldtype type of array element
/// @param newtype name of the new type
/// @return 
int MPI_Type_create_subarray(
        int ndims, int sizes[], int subsizes[], int starts[],
        int order, MPI_Datatype oldtype, MPI_Datatype *newtype){}

int MPI_Type_commit(MPI_Datatype* newtype){return 0;}

/// @brief 
/// @param to_free 
/// @return 
int MPI_Type_free(MPI_Datatype* to_free){return 0;}

/// @brief 
/// @param comm 
/// @return 
int MPI_Comm_free(MPI_Comm *comm){return 0;}

//int MPI_BSEND_OVERHEAD = 0;


#endif // PSEUDO_MPI
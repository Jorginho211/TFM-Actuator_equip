#ifndef WorkersList_h_
#define WorkersList_h_


class WorkersList {
    public:
        char **workers;
        unsigned int size;

        WorkersList() {
            this->size = 0;
        }

        WorkersList(unsigned int arraySize, unsigned int uuidSize) {
            this->size = arraySize;
            workers = new char*[this->size];
            for(unsigned int i = 0; i < this->size; i++){
                workers[i] = new char[uuidSize];
            }
        }

        bool contains(std::string workerUuid) {
            if(this->workers == NULL || this->size == 0){
                return false;
            }

            for(unsigned int i = 0; i < this->size; i++){
                if(strcmp(this->workers[i], workerUuid.c_str()) == 0){
                    return true;
                }
            }

            return false;
        }

        void dispose(){
            if(this->workers == NULL || this->size == 0){
                return;
            }

            for(unsigned int i = 0; i < this->size; i++){
                delete []this->workers[i];
            }

            delete []this->workers;
        }

        void toString() {
            if(this->workers == NULL || this->size == 0){
                return;
            }

            printf("------------------------------------------\n");
            for(unsigned int i = 0; i < this->size; i++){
                printf("%s\n", this->workers[i]);
            }
            printf("------------------------------------------\n");
        }
};
#endif

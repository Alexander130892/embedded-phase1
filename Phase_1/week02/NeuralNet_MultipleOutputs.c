/*
 * File:    NeuralNet_MultipleOutputs.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This C file implements a neural network with one hidden layer that
 *   simultaneously learns to compute five different Boolean logic
 *   operations (OR, XOR, NOR, AND, NAND) from the same two binary
 *   inputs. The network uses backpropagation with sigmoid activation
 *   to train on all four possible input combinations until achieving a
 *   90% success rate across all operations.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <math.h>

//Consts
#define N_INPUTS 2
#define HIDDEN_LAYER 6
#define N_OUTPUTS 5 // OR - XOR - NOR - AND - NAND
#define LEARNING_RATE 0.01
#define MAX_EPOCHS 100000
#define N_POSSIBILITES 4 //4 possibilites for Input pairs
#define SUCCES_RATE 0.9

int possible_inputs[N_POSSIBILITES][N_INPUTS] = { {0,0} , {0,1} , {1,0}, {1,1} }; 

//target outputs
int targets[N_POSSIBILITES][N_OUTPUTS] = {
    {0, 0, 1, 0, 1},  // A=0 B=0 → OR = 0 - XOR = 0 - NOR = 1 - AND = 0 - NAND = 1
    {1, 1, 0, 0, 1},  // A=0 B=1 → OR = 1 - XOR = 1 - NOR = 0 - AND = 0 - NAND = 1
    {1, 1, 0, 0, 1},  // A=1 B=0 → OR = 1 - XOR = 1 - NOR = 0 - AND = 0 - NAND = 1
    {1, 0, 0, 1, 0}   // A=1 B=1 → OR = 1 - XOR = 0 - NOR = 0 - AND = 1 - NAND = 0
};

//Typedefs
typedef struct NeuralNet{
    double weights_in[HIDDEN_LAYER][N_INPUTS];
    double weights_out[N_OUTPUTS][HIDDEN_LAYER];
    double hidden_output[HIDDEN_LAYER]; //needed for backwards propagation
    double bias_hidden[HIDDEN_LAYER];
    double bias_out[N_OUTPUTS];
} NeuralNet;

//Prototype Functions
//NeuralNet functions
void initNeuralNet(NeuralNet *network);
void printNeuralNet(NeuralNet *network);
void train(NeuralNet * network);
void predict(NeuralNet *network, int inputs[N_INPUTS], double predictions[N_OUTPUTS], double activation(double));
void trainNeuralNet(NeuralNet *network);
void testNeuralNet(NeuralNet *p_network);
void inference(double predictions[N_OUTPUTS], int inferences[N_OUTPUTS]);
//possible activation fcts (only sigmoid used)
double step(double x); //unused
double ReLU(double x); //unused
double sigmoid(double x);
// possible error fcts (unused)
double cross_entropy(double target, double pred);
double er(double x, double y);
double abs_er(double x, double y);

//MAIN
int main(){
    //Create NN
    NeuralNet network;
    //Init NN
    initNeuralNet(&network);  
    // train network
    printf("-----------Training----------------\n");
    trainNeuralNet(&network);
    printf("--------After Training--------------\n");
    //again
    printNeuralNet(&network);
    testNeuralNet(&network);
    printf("-------------------------------------\n");
    return EXIT_SUCCESS;
}

//Helper-Functions
void initNeuralNet(NeuralNet *network){
    srand(time(NULL));
    //init first layer
    for(int hidden_i = 0; hidden_i < HIDDEN_LAYER ; hidden_i++){
        for(int weight_i = 0; weight_i < N_INPUTS ; weight_i++)
        {
            network->weights_in[hidden_i][weight_i] = (rand()%20)/10.0-1; // [-1,1]
        }
    }
    //init hidden layer
    for(int output_i = 0; output_i < N_OUTPUTS ; output_i++){
        for(int hidden_i = 0; hidden_i < HIDDEN_LAYER ; hidden_i++)
        {
            network->weights_out[output_i][hidden_i] = (rand()%20)/10.0-1; // [-1,1]
        }
    }
    //biases
    for(int neuron_i = 0; neuron_i < HIDDEN_LAYER ; neuron_i++)
    {
        network->bias_hidden[neuron_i] = (rand()%20)/10.0-1; // [-1,1]
    }
    for(int neuron_i = 0; neuron_i < N_OUTPUTS ; neuron_i++)
    {
        network->bias_out[neuron_i] = (rand()%20)/10.0-1; // [-1,1]
    }
}
void printNeuralNet(NeuralNet *network){
    printf("Weights INPUT <--> HIDDEN_LAYER: ");
    for(int input_i = 0; input_i < N_INPUTS ; input_i++)
    {   
        putchar('\n');
        for(int hidden_j = 0; hidden_j < HIDDEN_LAYER ; hidden_j++)
        {
            printf("w%d%d: %2.2f  ",hidden_j, input_i, network->weights_in[hidden_j][input_i]);
        }
    }
    putchar('\n');
    printf("Weights HIDDEN_LAYER <--> OUTPUT: ");
    for(int hidden_j = 0; hidden_j < HIDDEN_LAYER ; hidden_j++)
    {   
        putchar('\n');
        for(int output_i = 0; output_i < N_OUTPUTS ; output_i++)
        {
            printf("w%d%d: %2.2f  ",output_i, hidden_j, network->weights_out[output_i][hidden_j]);
        }
    }
    putchar('\n');
    printf("Biases Hidden layer:  ");
    for(int neuron_i = 0; neuron_i < HIDDEN_LAYER ; neuron_i++)
    {
       printf("%2.1f  ",network->bias_hidden[neuron_i]); 
    }
    putchar('\n');
    printf("Biases Output layer:  ");
    for(int neuron_i = 0; neuron_i < N_OUTPUTS ; neuron_i++)
    {
       printf("%2.1f  ",network->bias_out[neuron_i]); 
    }
    putchar('\n');
}
void predict(NeuralNet *network, int* inputs, double* predictions, double activation(double)){
    double temp;
    //Forward pass IN --> HIDDEN
    for (int hidden_i = 0 ; hidden_i < HIDDEN_LAYER ; hidden_i++){
        temp=0.0;
        for(int input_i=0; input_i < N_INPUTS ; input_i++)
        {
            temp+=inputs[input_i]*network->weights_in[hidden_i][input_i];
        }
        temp+=network->bias_hidden[hidden_i];
        network->hidden_output[hidden_i] =activation(temp);
    }
    //Forward pass HIDDEN --> OUT
    for(int prediction_i=0; prediction_i < N_OUTPUTS ; prediction_i++)
    {
        temp=0.0;
        for (int hidden_i = 0 ; hidden_i < HIDDEN_LAYER ; hidden_i++)
        {
            temp+=network->hidden_output[hidden_i]*network->weights_out[prediction_i][hidden_i];
        }
        temp+=network->bias_out[prediction_i];
        predictions[prediction_i] =activation(temp);
        //predictions[prediction_i] = (int) round(temp); dont map to 0/1 in prediction --> inference function
    }
    return;
}
void trainNeuralNet(NeuralNet *network){
    double current_pred[N_OUTPUTS];
    double hidden_error[HIDDEN_LAYER];
    int correct=0;
    int count;
    for (count=1; ((double)correct/N_POSSIBILITES) < SUCCES_RATE && count < MAX_EPOCHS; count++){
        double error[N_OUTPUTS]={0};
        correct = 0;
        // run NN w/ current weights and get result
        for(int possible_input = 0 ; possible_input < N_POSSIBILITES ; possible_input++ ){
            predict(network, possible_inputs[possible_input], current_pred, sigmoid);
            // compute all errors
            for(int output_i=0; output_i < N_OUTPUTS ; output_i++){
                error[output_i] = targets[possible_input][output_i] - current_pred[output_i]; 
            }
            // With 5 outputs you need to count a sample as correct only when all outputs match
            int sample_correct = 1;
            for(int output_j = 0; output_j < N_OUTPUTS; output_j++){
                if((int)round(current_pred[output_j]) != targets[possible_input][output_j])
                    sample_correct = 0;
            }
            correct += sample_correct;
            //Backwards propogation of error in attempt to correct the weights
            // HIDDEN --> OUT
            double d_hidden;
            for(int hidden_i = 0; hidden_i < HIDDEN_LAYER; hidden_i++){
                hidden_error[hidden_i] = 0.0;  // reset before accumulating
                d_hidden = network->hidden_output[hidden_i] * (1 - network->hidden_output[hidden_i]);
            }
            for(int output_j=0; output_j < N_OUTPUTS ; output_j++)
            {
                for (int hidden_i = 0 ; hidden_i < HIDDEN_LAYER ; hidden_i++)
                {
                    hidden_error[hidden_i] += error[output_j] * network->weights_out[output_j][hidden_i] * d_hidden;
                    network->weights_out[output_j][hidden_i] += error[output_j] * LEARNING_RATE * network->hidden_output[hidden_i];
                }
            }
            // IN --> HIDDEN
            for (int hidden_i = 0 ; hidden_i < HIDDEN_LAYER ; hidden_i++){
                for(int input_i=0; input_i < N_INPUTS ; input_i++)
                {
                    network->weights_in[hidden_i][input_i]+= hidden_error[hidden_i] * LEARNING_RATE * possible_inputs[possible_input][input_i];
                }
            }
            for (int bias_i =0 ; bias_i < HIDDEN_LAYER ; bias_i++){
                network->bias_hidden[bias_i]+= hidden_error[bias_i] * LEARNING_RATE;
            }
            for (int bias_i =0 ; bias_i < N_OUTPUTS ; bias_i++){
                network->bias_out[bias_i]+= error[bias_i] * LEARNING_RATE;
            }
        }
        // if(count % 100==0)
        //     printf("iter %d | w0=%.4f w1=%.4f b=%.4f\n", count, network->weights[0], network->weights[1], network->bias[0]);
    }
    if((double)correct/N_POSSIBILITES >= SUCCES_RATE)
        printf("Converged in %d epochs\n", count-1);
    else
        printf("Failed to converge after %d epochs\n", count-1);
    // --> result: Improved network
}
void inference(double predictions[N_OUTPUTS], int inferences[N_OUTPUTS]){
    //Map predictions to potenial outputs
    for (int index = 0 ; index < N_OUTPUTS ; index++){
        inferences[index] = (int) round(predictions[index]);
    }
    return;
}
void testNeuralNet(NeuralNet *p_network){
    double predictions[N_OUTPUTS];
    int possible_inputs[N_INPUTS];
    int inferences[N_OUTPUTS];
    printf("\tExpected Output:  %5s%5s%5s%5s%5s\n","OR", "XOR", "NOR", "AND", "NAND");
    for (int input_1=0 ; input_1 <= 1 ; input_1++)
    {
        for (int input_2=0 ; input_2 <= 1 ; input_2++)
        {
            possible_inputs[0] = input_1;
            possible_inputs[1] = input_2;
            predict(p_network, possible_inputs, predictions, sigmoid);
            inference(predictions, inferences);
            
            printf("The output of(%d , %d) is: ",possible_inputs[0], possible_inputs[1]);
            for (int output_i=0; output_i < N_OUTPUTS ; output_i++){
                printf("%5d",inferences[output_i]);
            }
            putchar('\n');
        }
    }
}


//Activation functions
double sigmoid(double x){
    return 1.0/(1+pow(M_E, -x));
}
double step(double x){
    return (x>0)?1:0;
}
double ReLU(double x){
    return (x>0)?x:0;
}
//Error functions
double er(double x, double y){
    return (x-y);
}
double abs_er(double x, double y){
    return fabs(x-y);
}
double cross_entropy(double target, double pred){ // by using X-entropy as loss function, we dont need the gradient of sigmoid function (they cancel each other out)
    return -(target * log(pred) + (1-target) * log(1-pred));
}   // since we stop as soon as all solution are found (SUCCES_RATE), we dont actually use the loss function at all!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 16
#define MAX_PERCEPTRONS 256
#define THRESHOLD 32

// Perceptron structure
typedef struct {
    int weights[MAX_HISTORY + 1]; // +1 for the bias weight
} Perceptron;

// Global variables
Perceptron perceptrons[MAX_PERCEPTRONS];
int global_history[MAX_HISTORY];
int history_length;
int num_perceptrons;

// Initialize perceptrons and history
void initialize(int hist_len, int num_perc) {
    history_length = hist_len;
    num_perceptrons = num_perc;
    
    memset(global_history, 0, sizeof(global_history));

    for (int i = 0; i < num_perceptrons; i++) {
        for (int j = 0; j <= history_length; j++) {
            perceptrons[i].weights[j] = 0;
        }
    }
}

// Get the perceptron prediction
int predict(Perceptron *perc, int *history) {
    int sum = perc->weights[0]; // Bias weight
    for (int i = 0; i < history_length; i++) {
        sum += perc->weights[i + 1] * history[i];
    }
    return sum;
}

// Train the perceptron
void train(Perceptron *perc, int *history, int outcome) {
    int prediction = predict(perc, history);
    int y = (prediction >= 0) ? 1 : -1;

    // Update weights if prediction is wrong or close to threshold
    if (y != outcome || abs(prediction) <= THRESHOLD) {
        perc->weights[0] += outcome; // Update bias
        for (int i = 0; i < history_length; i++) {
            perc->weights[i + 1] += outcome * history[i];
        }
    }
}

// Update global history
void update_history(int outcome) {
    for (int i = history_length - 1; i > 0; i--) {
        global_history[i] = global_history[i - 1];
    }
    global_history[0] = outcome;
}

int main() {
    int hist_len, num_perc, num_branches;

    printf("Enter history length: ");
    scanf("%d", &hist_len);
    printf("Enter number of perceptrons: ");
    scanf("%d", &num_perc);

    initialize(hist_len, num_perc);

    printf("Enter number of branches to simulate: ");
    scanf("%d", &num_branches);

    for (int i = 0; i < num_branches; i++) {
        int branch_id, actual_outcome;
        printf("\nEnter branch ID (0-%d): ", num_perceptrons - 1);
        scanf("%d", &branch_id);

        printf("Enter actual outcome (1 for taken, -1 for not taken): ");
        scanf("%d", &actual_outcome);

        if (branch_id >= num_perceptrons) {
            printf("Invalid branch ID!\n");
            continue;
        }

        int prediction = predict(&perceptrons[branch_id], global_history);
        int predicted_outcome = (prediction >= 0) ? 1 : -1;

        printf("Prediction: %s\n", predicted_outcome == 1 ? "Taken" : "Not Taken");

        train(&perceptrons[branch_id], global_history, actual_outcome);
        update_history(actual_outcome);

        printf("Updated history: ");
        for (int j = 0; j < history_length; j++) {
            printf("%d ", global_history[j]);
        }
        printf("\n");
    }

    return 0;
}

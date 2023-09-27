# My Neural Network

Welcome to the "My Neural Network" project. This project aims to construct a neural network from scratch using C++, trained and tested initially on the provided datasets: `Grades` and `WDBC`. Additionally, as part of the project's expansion, a custom dataset related to NBA team statistics was integrated, serving as a real-world application of the neural network.

## Objective

The overarching goal is to understand and implement the intricacies of a neural network in C++ and apply it on various datasets to test its efficiency and accuracy.

## Getting Started

1. **Compiling the Code**:
    ```shell
    make
    ```
2. **Executing the Program**:
    ```shell
    ./Net
    ```

## Datasets

### Built-in Test Files:
- `Grades_TEXT_FILES`: Contains text files related to the `Grades` dataset.
- `WDBC_TEXT_FILES`: Holds files for the `WDBC` dataset.

### Custom NBA Dataset:

Originating from [NBA Advanced Stats](https://www.nba.com/stats/teams/advanced?Season=2021-22), this dataset spans 14 seasons and explores the likelihood of NBA teams winning over 50% of their games based on their season averages of advanced stat metrics.

#### Data Processing:
- From 19 original categories, the refined data houses 14 inputs and 1 output.
- The input metrics are standardized season-wise, while outputs are binarized based on win percentages.

All the meticulous data processing can be traced in the `NBA_RAW_DATA` Excel sheet under the `Excel_Sheets` folder.

After preparing the data, I segregated it into ~300 training and ~100 test cases, initialized a neural network, and executed the model.

#### Neural Network Dynamics:
- A neural network from the `WDBC` test case was adapted for this dataset.
- Optimal outcomes were produced with configurations of 5 Hidden Nodes, 500 Epochs, and a learning rate of 0.05.

Refer to the `NBA_OVER_50_Percent_Wins` folder for comprehensive details on the neural network setup, associated files, and results.

## Further Resources

An Excel spreadsheet showcasing results from diverse test iterations with variable epochs and learning rates is available in the `Excel_Sheets` folder.

## Contact & Feedback

For any further inquiries or potential oversights, please connect via email.

Thanks,
Evan Kluger

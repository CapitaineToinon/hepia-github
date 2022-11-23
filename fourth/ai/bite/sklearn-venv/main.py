import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.datasets import load_iris
from sklearn.tree import DecisionTreeClassifier
from sklearn.inspection import DecisionBoundaryDisplay
from sklearn.tree import plot_tree

training = "./data/students.csv"
testing = './data/test_data.csv'

students = pd.read_csv(training)
iris = load_iris()

target = students["success"].to_numpy()
data = students[["grade_1", "grade_2"]].to_numpy()
clf = DecisionTreeClassifier().fit(data, target)

plt.figure()
plot_tree(clf, filled=True)
plt.title("Decision tree trained on all the students")
plt.show()

test_data = pd.read_csv(testing)
test_target = test_data["success"].to_numpy()
test_data = test_data[["grade_1", "grade_2"]].to_numpy()

results = [[a, b, *clf.predict([a])] for a, b in zip(test_data, test_target)]

count = 0
total = len(results)
for [source, expected, actual] in results:
    status = 'OK' if expected == actual else ''
    count = count + 1 if status == 'OK' else count
    print(f"expect {source} to give {expected} got {actual} {status}")

print(f"{count}/{total} ({count/total*100:.1f}%)")

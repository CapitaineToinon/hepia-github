import csv
import enum
from random import choice
import matplotlib.pyplot as plt
import numpy as np


class Student:
    def __init__(self, row) -> None:
        [success, grade_1, grade_2] = row
        self.success = float(success)
        self.grade_1 = float(grade_1)
        self.grade_2 = float(grade_2)

    @staticmethod
    def plot(students: list['Student']):
        fig1, ax = plt.subplots()
        ax.scatter([s.grade_1 for s in students],
                   [s.grade_2 for s in students])
        ax.set_xlim(0, 100)
        ax.set_ylim(0, 100)
        ax.set_box_aspect(1)
        plt.show()


class StudentKMeans:
    def __init__(self,  k_count=2, max_iterations=300000) -> None:
        self.k_count = k_count
        self.max_iterations = max_iterations

    @staticmethod
    def manhattan(point: list[float], centroids: list[list[float]]):
        distances = []
        for centroid in centroids:
            distance = 0

            for i, el in enumerate(centroid):
                difference = point[i] - el
                absolute_difference = abs(difference)
                distance += absolute_difference

            distances.append(distance)
        return distances

    def do_the_thing(self, students: list['Student']) -> None:
        fig1, ax = plt.subplots()

        ax.set_xlim(0, 100)
        ax.set_ylim(0, 100)
        ax.set_box_aspect(1)

        data = [[s.grade_1, s.grade_2, s.success] for s in students]
        self.centroids = [choice(data) for _ in range(self.k_count)]
        prev_centroids: list[list[float]] = None

        iteration = 0
        while np.not_equal(self.centroids, prev_centroids).any() and iteration < self.max_iterations:
            sorted_points = [[] for _ in range(self.k_count)]

            for d in data:
                dists = StudentKMeans.manhattan(d, self.centroids)
                centroid_idx = np.argmin(dists)
                sorted_points[centroid_idx].append(d)

            prev_centroids = self.centroids
            self.centroids = [np.mean(cluster, axis=0)
                              for cluster in sorted_points]

            for i, centroid in enumerate(self.centroids):
                # Catch any np.nans, resulting from a centroid having no points
                if np.isnan(centroid).any():
                    self.centroids[i] = prev_centroids[i]

            iteration += 1

            centroids = []
            centroid_idxs = []
            for x in data:
                dists = StudentKMeans.manhattan(x, self.centroids)
                centroid_idx = np.argmin(dists)
                centroids.append(self.centroids[centroid_idx])
                centroid_idxs.append(centroid_idx)

            ax.scatter([s.grade_1 for s in students],
                       [s.grade_2 for s in students],
                       c=centroid_idxs)

            ax.plot([x for x, _, _ in centroids],
                    [y for _, y, _ in centroids],
                    'k+',
                    markersize=10,
                    )
            plt.pause(0.2)

        plt.show()


def from_csv(file) -> list['Student']:
    students = []
    skip = 1

    with open(file, newline='') as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',')
        for row in spamreader:
            if skip > 0:
                skip -= 1
                continue
            students.append(Student(row))

    return students


if __name__ == "__main__":
    students = from_csv('data/students.csv')
    kmean = StudentKMeans(k_count=2)
    kmean.do_the_thing(students=students)

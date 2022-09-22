import { Observer } from "./observer";

export abstract class Subject {
	#observers: Set<Observer> = new Set();

	attach(observer: Observer, options = { immediate: false }) {
		this.#observers.add(observer);
		if (options.immediate) observer.update();
	}

	detach(observer: Observer) {
		this.#observers.delete(observer);
	}

	notify() {
		this.#observers.forEach((observer) => observer.update());
	}
}

import { OutilCalorie } from "./Outil/outilCalorie";
import { Observer } from "../Observable/observer";

export class Fruit implements Observer {
	#outil: OutilCalorie;
	#name: string;
	#observerState: Map<string, number> | null = null;

	constructor({ name, outil }: { name: string; outil: OutilCalorie }) {
		this.#name = name;
		this.#outil = outil;
		this.#outil.attach(this, { immediate: true });
	}

	update(): void {
		this.#observerState = this.#outil.getState();
		console.log(
			`La valeur a changé pour le fruit ${
				this.#name
			}. Nouvelle valeures des calories: ${this.getCalories()}`
		);
	}

	getCalories(): number {
		return this.#observerState?.get(this.#name) ?? 0;
	}
}

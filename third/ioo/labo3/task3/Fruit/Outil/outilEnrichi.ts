import { OutilCalorie } from "./outilCalorie";

export class OutilEnrichi extends OutilCalorie {
	#table = new Map<string, number>([
		["pomme", 5000],
		["banane", 6000],
	]);

	getState(): Map<string, number> {
		return this.#table;
	}

	setState(state: Map<string, number>): void {
		this.#table = state;
		this.notify();
	}

	change() {
		this.setState(new Map([...this.#table, ["pomme", Math.random() * 1000]]));
	}
}

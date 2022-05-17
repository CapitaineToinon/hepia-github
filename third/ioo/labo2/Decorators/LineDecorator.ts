import { Fruit } from "../Fruits/Fruit";
import { Decorator } from "./Decorator";

export class LineDecorator extends Decorator {
	#char: string = "#";

	constructor({ fruit, char }: { fruit: Fruit; char?: string }) {
		super({ fruit });
		this.#char = char ?? this.#char;
	}

	get line(): string {
		return Array.from({ length: 30 })
			.map(() => this.#char)
			.join("");
	}

	override toString(): string {
		return this.line + "\n" + this.fruit.toString() + "\n" + this.line;
	}
}

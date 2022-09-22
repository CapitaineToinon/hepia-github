import { Subject } from "../../Observable/subject";

export abstract class OutilCalorie extends Subject {
	abstract change(): void; // used to trigger artifical changes
	abstract getState(): Map<string, number>;
	abstract setState(state: Map<string, number>): void;
}

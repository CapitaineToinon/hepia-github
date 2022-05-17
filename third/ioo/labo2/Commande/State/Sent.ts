import { Commande } from "../index";
import { State } from "./State";

export class Sent extends State {
	traiterCommande(commande: Commande) {
		console.log("Commande is sent. This state cannot change anymore");
		// commande cannot leave the Sent state once it's there
	}
}

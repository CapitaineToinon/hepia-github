class Vertex {
  name: string;
  source: boolean;
  sink: boolean;
  edges: Edge[];

  constructor(name: string, source = false, sink = false) {
    this.name = name;
    this.source = source;
    this.sink = sink;
    this.edges = [];
  }
}

class S extends Vertex {
  constructor(name: string = "s") {
    super(name, true, false);
  }
}

class T extends Vertex {
  constructor(name: string = "t") {
    super(name, false, true);
  }
}

class Edge {
  start: Vertex;
  end: Vertex;
  capacity: number;
  flow: number;

  constructor(start: Vertex, end: Vertex, capacity: number) {
    this.start = start;
    this.end = end;
    this.capacity = capacity;
    this.flow = 0;
  }

  residual(): number {
    return this.capacity - this.flow;
  }
}

// source
const source = new S();

// layer 1
const v1 = new Vertex("v1");
const v2 = new Vertex("v2");
const v3 = new Vertex("v3");

source.edges.push(new Edge(source, v1, 45));
source.edges.push(new Edge(source, v2, 25));
source.edges.push(new Edge(source, v3, 20));

// layer 2
const v4 = new Vertex("v4");
const v5 = new Vertex("v5");
const v6 = new Vertex("v6");
const v7 = new Vertex("v7");

v1.edges.push(new Edge(v1, v4, 10));
v1.edges.push(new Edge(v1, v5, 15));
v1.edges.push(new Edge(v1, v7, 20));

v2.edges.push(new Edge(v2, v4, 20));
v2.edges.push(new Edge(v2, v5, 5));
v2.edges.push(new Edge(v2, v6, 15));

v3.edges.push(new Edge(v3, v6, 10));
v3.edges.push(new Edge(v3, v7, 10));

// layer 3
const sink = new T();

v4.edges.push(new Edge(v4, sink, 30));
v5.edges.push(new Edge(v5, sink, 10));
v6.edges.push(new Edge(v6, sink, 20));
v7.edges.push(new Edge(v7, sink, 30));

function traverse(vertex: Vertex, path: Edge[] = []) {
  if (vertex.sink) {
    const minCapacity = Math.min(...path.map((edge) => edge.capacity));
    const minResidual = Math.min(...path.map((edge) => edge.residual()));
    const toAdd = Math.min(minCapacity, minResidual);
    path.forEach((edge) => (edge.flow += toAdd));

    /* code below is to print pretty comments explaining stuff */
    const vertices: Vertex[] = [];
    path.forEach((edge) => {
      vertices.push(edge.start);
      if (edge.end.sink) vertices.push(edge.end);
    });

    const prettyPath = `${vertices.map((v) => v.name).join(" -> ")}`;
    console.log(`🚛 📦 Updating the path ${prettyPath}`);
    if (toAdd) {
      console.log(`We find we can add a flow of ${toAdd} through this path`);
      console.log(`The updated path therefore looks like this now:`);
    } else {
      console.log(`This path is already full to its max capacity`);
    }

    path.map((edge) => {
      const flow = `${edge.start.name} -> ${edge.flow}/${edge.capacity} (+${toAdd}) -> ${edge.end.name}`;
      console.log(flow);
    });

    console.log("And residual for this path is now:");
    path.map((edge) => {
      console.log(`${edge.start.name} <- ${edge.flow} <- ${edge.end.name}`);
      console.log(
        `${edge.start.name} -> ${edge.residual()} -> ${edge.end.name}`
      );
    });
    /* end of pretty print */
  }

  vertex.edges.forEach((edge) => {
    traverse(edge.end, [...path, edge]);
  });
}

function computeFlow(vertex: Vertex) {
  function findFinalEdges(vertex: Vertex) {
    let finalEdges = new Set<Edge>();

    vertex.edges.forEach((edge) => {
      if (edge.end.sink) finalEdges.add(edge);
      else findFinalEdges(edge.end).forEach((edge) => finalEdges.add(edge));
    });

    return finalEdges;
  }

  const startingEdges = vertex.edges;
  const finalEdges = [...findFinalEdges(vertex)];

  const intialCapacity = startingEdges.reduce(
    (previous, current) => (previous += current.capacity),
    0
  );

  const actualFlow = finalEdges.reduce(
    (previous, current) => (previous += current.flow),
    0
  );

  console.log(
    `👉 The system had total potential capacity of ${intialCapacity} ${
      intialCapacity === actualFlow ? "and" : "but only"
    } ${actualFlow} can flow through it`
  );
}

traverse(source);

computeFlow(source);

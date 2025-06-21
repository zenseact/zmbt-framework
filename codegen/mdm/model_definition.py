# (c) Copyright 2025 Zenseact AB
# SPDX-License-Identifier: Apache-2.0

TERMINAL_NODE = 'N_Term'

class Transition:
    def __init__(self, cls, target):
        self.Class = cls
        self.Target = target

    def  __eq__(self, other):
        return self.Class == other.Class and self.Target == other.Target

    def __hash__(self):
        return hash(self.Class + self.Target)


class Node:
    def __init__(self, cls, base, transitions):
        self.Class = cls
        self.BaseNode = base
        self.Transitions = tuple(Transition(trans, trg) for trans, trg in transitions.items())

        sep = "\n    , "
        self.BasesList = sep.join(map(lambda t: f"public ModelDefinition::{t.Class}<ModelDefinition::{t.Target}>",  self.Transitions))
        if self.BaseNode:
            self.BasesList = f"public ModelDefinition::{self.BaseNode}" + sep + self.BasesList

        if self.BaseNode or self.Transitions:
            self.BasesList = f"\n    : " + self.BasesList

class DefinitionMachine:
    def __init__(self, data: dict):
        nodes = data.get('nodes')
        self.Nodes = tuple(Node(cls, n['base'], n['trans']) for cls, n in nodes.items() if cls != TERMINAL_NODE)
        self.Transitions = tuple(dict.fromkeys(t for n in self.Nodes for t in n.Transitions))
        self.AbstractTransitions = tuple(sorted(set(t.Class for n in self.Nodes for t in n.Transitions)))

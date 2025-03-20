# (c) Copyright 2025 Zenseact AB
# SPDX-License-Identifier: Apache-2.0


class Transition:
    def __init__(self, cls, source, target):
        self.Class = cls
        self.Source = source
        self.Target = target

class Node:
    def __init__(self, cls, base, transitions):
        self.Class = cls
        self.Base = base
        self.Transitions = tuple(Transition(t, cls, trg) for t, trg in transitions.items())

class DefinitionMachine:
    def __init__(self, data: dict):
        nodes = data.get('nodes')
        self.Nodes = tuple(Node(cls, n['base'], n['transitions']) for cls, n in nodes.items())
        self.Transitions = tuple(t for n in self.Nodes for t in n.Transitions)
        self.AbstractTransitions = tuple(sorted(set(t.Class for n in self.Nodes for t in n.Transitions if t.Class != 'T_Null')))

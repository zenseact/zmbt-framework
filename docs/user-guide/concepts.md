Framework Concepts
============


- **Channel**: Specification for addressing the test stimuli or responses. Channel parameters include *role* (stimulus or response), *interface* (callable reference), *kind* (arguments, return value, exception, or call count), *signal path*, *call* (number of invocation).
- **Environment**: A globally available registry containing all test stimuli and observed responses.
- **Interface**: Function, member function with host object, or callable object, referenced directly (with C++ pointers or refs) or by string key (if registered in the *Environment*).
- **Model**: SUT abstraction focusing on verification of particular feature.
- **Mock**: Test double for callable that reroute any interactions to the *Environment*.
- **Response**: observable signal on SUT
- **Stimulus**: signal affecting the SUT
- **SUT**: System under test
- **Trigger**: A safe SUT execution wrapper for callable that feeds stimuli and captures responses in serialized form, enabling rerouting to the *Environment*.
- **Test Vector**: A list of test values - stimuli signals and expectation matchers for a single test case.
- **Tasks**: Test setup and cleanup callbacks.

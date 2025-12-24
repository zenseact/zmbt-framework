
# Class List


Here are the classes, structs, unions and interfaces with brief descriptions:

* **namespace** [**zmbt**](namespacezmbt.md) _ZMBT root namespace._     
    * **class** [**Config**](classzmbt_1_1Config.md) _Global app config._     
    * **class** [**Environment**](classzmbt_1_1Environment.md) _Controlled environment data storage._     
        * **class** [**InterfaceHandle**](classzmbt_1_1Environment_1_1InterfaceHandle.md) [_**Environment**_](classzmbt_1_1Environment.md) _extension handling the interface data._    
        * **class** [**TypedInterfaceHandle**](classzmbt_1_1Environment_1_1TypedInterfaceHandle.md) [_**Environment**_](classzmbt_1_1Environment.md) _API handler for specific interface._    
    * **struct** [**EnvironmentData**](structzmbt_1_1EnvironmentData.md) _Data container for the_ [_**Environment**_](classzmbt_1_1Environment.md) _._    
    * **struct** [**ErrorInfo**](structzmbt_1_1ErrorInfo.md)     
    * **class** [**ErrorOr**](classzmbt_1_1ErrorOr.md)     
    * **class** [**Generator**](classzmbt_1_1Generator.md)     
    * **class** [**InjectionTable**](classzmbt_1_1InjectionTable.md)     
        * **struct** [**Record**](structzmbt_1_1InjectionTable_1_1Record.md)     
    * **class** [**JsonIter**](classzmbt_1_1JsonIter.md) _Boost JSON array iterator interface._     
    * **class** [**JsonIterZipProdBase**](classzmbt_1_1JsonIterZipProdBase.md) _Boost JSON array iterator base for zip or cartesian product._     
    * **class** [**JsonNode**](classzmbt_1_1JsonNode.md) _Mutator/accessor wrapper over shared JSON value._     
    * **class** [**JsonProdIter**](classzmbt_1_1JsonProdIter.md) _Cartesian Product Iterator._     
    * **class** [**JsonTraverse**](classzmbt_1_1JsonTraverse.md) _Boost JSON value depth-first traverser._     
    * **class** [**JsonZipIter**](classzmbt_1_1JsonZipIter.md) _Zip Iterator._     
    * **class** [**Logger**](classzmbt_1_1Logger.md)     
    * **class** [**OutputRecorder**](classzmbt_1_1OutputRecorder.md)     
        * **struct** [**Frame**](structzmbt_1_1OutputRecorder_1_1Frame.md)     
    * **class** [**Param**](classzmbt_1_1Param.md) _Generic Parameter Placeholder._     
    * **struct** [**PermanentEnvData**](structzmbt_1_1PermanentEnvData.md)     
    * **struct** [**StubLogger**](structzmbt_1_1StubLogger.md)     
    * **class** [**Trigger**](classzmbt_1_1Trigger.md) _Controlled environment interface executor._     
    * **class** [**TriggerIfc**](classzmbt_1_1TriggerIfc.md)     
    * **class** [**TriggerObj**](classzmbt_1_1TriggerObj.md) _Object handler to be used with_ [_**TriggerIfc**_](classzmbt_1_1TriggerIfc.md) _._    
    * **namespace** [**api**](namespacezmbt_1_1api.md) _Framework API with fixture classes and entries from main ns._     
        * **class** [**Environment**](classzmbt_1_1api_1_1Environment.md) _Alias for_ [_**zmbt::Environment**_](classzmbt_1_1Environment.md) _._
        * **class** [**Expression**](classzmbt_1_1api_1_1Expression.md) _Alias for_ [_**lang::Expression**_](classzmbt_1_1lang_1_1Expression.md) _._
        * **struct** [**ModelTestFixture**](structzmbt_1_1api_1_1ModelTestFixture.md) _Generic model-driven test fixture._     
        * **class** [**Param**](classzmbt_1_1api_1_1Param.md) _Alias for_ [_**zmbt::Param**_](classzmbt_1_1Param.md) _._
        * **class** [**SignalMapping**](classzmbt_1_1api_1_1SignalMapping.md) _Alias for_ [_**zmbt::mapping::SignalMapping**_](classzmbt_1_1mapping_1_1SignalMapping.md) _._
        * **namespace** [**decor**](namespacezmbt_1_1api_1_1decor.md) _Signal type decorators API._ 
        * **namespace** [**expr**](namespacezmbt_1_1api_1_1expr.md) [_**Expression**_](classzmbt_1_1api_1_1Expression.md) _Language API._
        * **class** [**precise**](classzmbt_1_1api_1_1precise.md) _Alias for zmbt::precise._ 
    * **namespace** [**application**](namespacezmbt_1_1application.md) _Test application configuration (work in progress)_ 
    * **struct** [**base\_error**](structzmbt_1_1base__error.md) _ZMBT Base exception._     
    * **namespace** [**decor**](namespacezmbt_1_1decor.md) _Signal type decorators API._     
        * **struct** [**precise**](structzmbt_1_1decor_1_1precise.md) _Type decorator for floating point numbers. Throws on precision loss in runtime._     
        * **struct** [**precision\_loss\_error**](structzmbt_1_1decor_1_1precision__loss__error.md) _Exception thrown by_ [_**decor::precise**_](structzmbt_1_1decor_1_1precise.md) _._
        * **struct** [**underlying**](structzmbt_1_1decor_1_1underlying.md)     
    * **class** [**entity\_id**](classzmbt_1_1entity__id.md) _Base class for annotated key objects._     
    * **struct** [**environment\_error**](structzmbt_1_1environment__error.md) _Test environment error._     
    * **namespace** [**expr**](namespacezmbt_1_1expr.md) _Expression Language API._     
    * **namespace** [**flags**](namespacezmbt_1_1flags.md)     
        * **class** [**ConversionTime**](classzmbt_1_1flags_1_1ConversionTime.md)     
        * **class** [**InjectionTime**](classzmbt_1_1flags_1_1InjectionTime.md)     
        * **class** [**RecordingTime**](classzmbt_1_1flags_1_1RecordingTime.md)     
        * **class** [**TestIsRunning**](classzmbt_1_1flags_1_1TestIsRunning.md)     
    * **struct** [**has\_bit\_and**](structzmbt_1_1has__bit__and.md) 
    * **struct** [**has\_bit\_or**](structzmbt_1_1has__bit__or.md) 
    * **struct** [**has\_bit\_xor**](structzmbt_1_1has__bit__xor.md) 
    * **struct** [**has\_complement**](structzmbt_1_1has__complement.md) 
    * **struct** [**has\_divides**](structzmbt_1_1has__divides.md) 
    * **struct** [**has\_equal\_to**](structzmbt_1_1has__equal__to.md) 
    * **struct** [**has\_greater**](structzmbt_1_1has__greater.md) 
    * **struct** [**has\_greater\_equal**](structzmbt_1_1has__greater__equal.md) 
    * **struct** [**has\_left\_shift**](structzmbt_1_1has__left__shift.md) 
    * **struct** [**has\_less**](structzmbt_1_1has__less.md) 
    * **struct** [**has\_less\_equal**](structzmbt_1_1has__less__equal.md) 
    * **struct** [**has\_logical\_and**](structzmbt_1_1has__logical__and.md) 
    * **struct** [**has\_logical\_not**](structzmbt_1_1has__logical__not.md) 
    * **struct** [**has\_logical\_or**](structzmbt_1_1has__logical__or.md) 
    * **struct** [**has\_minus**](structzmbt_1_1has__minus.md) 
    * **struct** [**has\_modulus**](structzmbt_1_1has__modulus.md) 
    * **struct** [**has\_multiplies**](structzmbt_1_1has__multiplies.md) 
    * **struct** [**has\_negate**](structzmbt_1_1has__negate.md) 
    * **struct** [**has\_not\_equal\_to**](structzmbt_1_1has__not__equal__to.md) 
    * **struct** [**has\_plus**](structzmbt_1_1has__plus.md) 
    * **struct** [**has\_right\_shift**](structzmbt_1_1has__right__shift.md) 
    * **struct** [**has\_serialization**](structzmbt_1_1has__serialization.md) 
    * **struct** [**ifc\_args**](structzmbt_1_1ifc__args.md)     
    * **struct** [**ifc\_handle**](structzmbt_1_1ifc__handle.md)     
    * **struct** [**ifc\_host**](structzmbt_1_1ifc__host.md)     
    * **struct** [**ifc\_pointer**](structzmbt_1_1ifc__pointer.md)     
    * **struct** [**ifc\_return**](structzmbt_1_1ifc__return.md)     
    * **class** [**interface\_id**](classzmbt_1_1interface__id.md) _Pointer-based interface id with type annotation._     
    * **struct** [**is\_const\_iterable**](structzmbt_1_1is__const__iterable.md) _Variable template that checks if a type has begin() and end() member functions._ 
    * **namespace** [**lang**](namespacezmbt_1_1lang.md) [_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _Language implementation._    
        * **struct** [**Encoding**](structzmbt_1_1lang_1_1Encoding.md)     
        * **class** [**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md)     
            * **struct** [**ExprRow**](structzmbt_1_1lang_1_1EncodingView_1_1ExprRow.md)     
            * **class** [**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md)     
        * **struct** [**EvalContext**](structzmbt_1_1lang_1_1EvalContext.md) [_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _evaluation context._    
        * **struct** [**EvalImpl**](structzmbt_1_1lang_1_1EvalImpl.md) 
        * **class** [**EvalImplBase**](classzmbt_1_1lang_1_1EvalImplBase.md)     
        * **struct** [**EvalLog**](structzmbt_1_1lang_1_1EvalLog.md) [_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _evaluation log._    
        * **class** [**EvalParams**](classzmbt_1_1lang_1_1EvalParams.md)     
        * **struct** [**EvalValidator**](structzmbt_1_1lang_1_1EvalValidator.md)     
        * **class** [**Expression**](classzmbt_1_1lang_1_1Expression.md)     
            * **class** [**Subexpression**](classzmbt_1_1lang_1_1Expression_1_1Subexpression.md)     
        * **struct** [**ExpressionGrammar**](structzmbt_1_1lang_1_1ExpressionGrammar.md)     
        * **class** [**ExpressionView**](classzmbt_1_1lang_1_1ExpressionView.md)     
        * **struct** [**FixedEvalParams**](structzmbt_1_1lang_1_1FixedEvalParams.md)     
        * **class** [**GlobalEnv**](classzmbt_1_1lang_1_1GlobalEnv.md) _Global Storage env for_ [_**Expression**_](classzmbt_1_1lang_1_1Expression.md) _side effects._    
        * **struct** [**KeywordGrammar**](structzmbt_1_1lang_1_1KeywordGrammar.md) _Boost.Sirit.Qi Grammar for_ [_**zmbt::lang::Keyword**_](namespacezmbt_1_1lang.md#enum-keyword) _parsing._    
        * **struct** [**KeywordInfo**](structzmbt_1_1lang_1_1KeywordInfo.md)     
        * **struct** [**KeywordSymbol**](structzmbt_1_1lang_1_1KeywordSymbol.md) _Boost.Sirit.Qi Symbol for_ [_**zmbt::lang::Keyword**_](namespacezmbt_1_1lang.md#enum-keyword) _parsing._    
        * **class** [**LazyParam**](classzmbt_1_1lang_1_1LazyParam.md)     
        * **class** [**Operator**](classzmbt_1_1lang_1_1Operator.md) _Signal transformation and comparison handler. Enables type erasure._     
            * **struct** [**operator\_error**](structzmbt_1_1lang_1_1Operator_1_1operator__error.md)     
        * **struct** [**SignatureBase**](structzmbt_1_1lang_1_1SignatureBase.md)     
        * **struct** [**SignatureBinary**](structzmbt_1_1lang_1_1SignatureBinary.md) _Binary expression._     
        * **struct** [**SignatureBinary&lt; Keyword::PreProc &gt;**](structzmbt_1_1lang_1_1SignatureBinary_3_01Keyword_1_1PreProc_01_4.md)     
        * **struct** [**SignatureCast**](structzmbt_1_1lang_1_1SignatureCast.md)     
        * **struct** [**SignatureConst**](structzmbt_1_1lang_1_1SignatureConst.md) _Const expression._ 
        * **struct** [**SignatureErr**](structzmbt_1_1lang_1_1SignatureErr.md)     
        * **struct** [**SignatureOp**](structzmbt_1_1lang_1_1SignatureOp.md)     
        * **struct** [**SignatureUnary**](structzmbt_1_1lang_1_1SignatureUnary.md) _Unary expression._ 
        * **struct** [**SignatureUncast**](structzmbt_1_1lang_1_1SignatureUncast.md)     
        * **struct** [**SignatureVariadic**](structzmbt_1_1lang_1_1SignatureVariadic.md) _Variadic expression._     
        * **namespace** [**attr**](namespacezmbt_1_1lang_1_1attr.md)     
    * **namespace** [**mapping**](namespacezmbt_1_1mapping.md) _Signal Mapping model functionality._     
        * **class** [**ChannelHandle**](classzmbt_1_1mapping_1_1ChannelHandle.md) _Channel clause handle for_ [_**SignalMapping**_](classzmbt_1_1mapping_1_1SignalMapping.md) _model._    
        * **class** [**ModelDefinition**](classzmbt_1_1mapping_1_1ModelDefinition.md) _Mapping model definition machine._     
            * **class** [**BaseTransition**](classzmbt_1_1mapping_1_1ModelDefinition_1_1BaseTransition.md)     
            * **class** [**N\_Channel**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Channel.md)     
            * **class** [**N\_ChannelEnd**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__ChannelEnd.md)     
            * **class** [**N\_CnlTag**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__CnlTag.md)     
            * **class** [**N\_Decor**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Decor.md)     
            * **class** [**N\_Descr**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Descr.md)     
            * **class** [**N\_Filter**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Filter.md)     
            * **class** [**N\_Main**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Main.md)     
            * **class** [**N\_MaybeChannel**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__MaybeChannel.md)     
            * **class** [**N\_Param**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Param.md)     
            * **class** [**N\_ParamTable**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__ParamTable.md)     
            * **class** [**N\_PipeId**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__PipeId.md)     
            * **class** [**N\_Post**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Post.md)     
            * **class** [**N\_Pre**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Pre.md)     
            * **class** [**N\_Repeat**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Repeat.md)     
            * **class** [**N\_Take**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Take.md)     
            * **class** [**N\_Term**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Term.md)     
            * **class** [**N\_Test**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__Test.md)     
            * **class** [**N\_TestTable**](classzmbt_1_1mapping_1_1ModelDefinition_1_1N__TestTable.md)     
        * **class** [**PipeHandle**](classzmbt_1_1mapping_1_1PipeHandle.md) _Pipe clause handle for_ [_**SignalMapping**_](classzmbt_1_1mapping_1_1SignalMapping.md) _model._    
        * **class** [**SignalMapping**](classzmbt_1_1mapping_1_1SignalMapping.md) _Signal Mapping Test Model._     
        * **struct** [**TestDiagnostics**](structzmbt_1_1mapping_1_1TestDiagnostics.md) _Test diagnostics for test case execution._     
        * **class** [**TestParameterResolver**](classzmbt_1_1mapping_1_1TestParameterResolver.md) _Test spec generator that handles parameter resolution._     
        * **class** [**TestRunner**](classzmbt_1_1mapping_1_1TestRunner.md) _Test runner for_ [_**SignalMapping**_](classzmbt_1_1mapping_1_1SignalMapping.md) _model._    
    * **struct** [**model\_error**](structzmbt_1_1model__error.md) _Model definition error._     
    * **class** [**object\_id**](classzmbt_1_1object__id.md) _Pointer-based object id with type annotation._     
        * **struct** [**string\_key**](structzmbt_1_1object__id_1_1string__key.md) 
    * **struct** [**output\_recorder\_error**](structzmbt_1_1output__recorder__error.md)     
    * **namespace** [**reflect**](namespacezmbt_1_1reflect.md) _Reflection metafunctions._     
        * **class** [**Prototypes**](classzmbt_1_1reflect_1_1Prototypes.md) _Interface prototypes in JSON as {"args": [...], "return": ... }._     
        * **struct** [**custom\_invocation**](structzmbt_1_1reflect_1_1custom__invocation.md) _Customize the interface reflection metafunction._ 
        * **struct** [**custom\_serialization**](structzmbt_1_1reflect_1_1custom__serialization.md) _User-defined serialization._ 
        * **struct** [**custom\_serialization&lt; ChannelKind &gt;**](structzmbt_1_1reflect_1_1custom__serialization_3_01ChannelKind_01_4.md)     
        * **struct** [**custom\_serialization&lt; T, mp\_if&lt; is\_base\_of&lt; lang::Expression, T &gt;, void &gt; &gt;**](structzmbt_1_1reflect_1_1custom__serialization_3_01T_00_01mp__if_3_01is__base__of_3_01lang_1_1Ex2d769b9dfa1b415c78e21b0b59756cc2.md)     
        * **struct** [**custom\_serialization&lt; lang::Encoding &gt;**](structzmbt_1_1reflect_1_1custom__serialization_3_01lang_1_1Encoding_01_4.md)     
        * **struct** [**custom\_serialization&lt; lang::ExpressionView &gt;**](structzmbt_1_1reflect_1_1custom__serialization_3_01lang_1_1ExpressionView_01_4.md)     
        * **struct** [**custom\_signal\_traits**](structzmbt_1_1reflect_1_1custom__signal__traits.md) _Customize signal traits metafunction._ 
        * **struct** [**default\_invocation**](structzmbt_1_1reflect_1_1default__invocation.md) _Interface reflection metafunction._ 
        * **struct** [**default\_invocation&lt; S, first\_if\_any\_t&lt; void, ifc\_is\_fn\_handle&lt; S &gt;, ifc\_is\_functor\_ref&lt; S &gt; &gt; &gt;**](structzmbt_1_1reflect_1_1default__invocation_3_01S_00_01first__if__any__t_3_01void_00_01ifc__is_03ee2f30da4b8a63c386d237830e07c9.md)     
        * **struct** [**default\_invocation&lt; S, first\_if\_t&lt; void, ifc\_is\_functor\_ptr&lt; S &gt; &gt; &gt;**](structzmbt_1_1reflect_1_1default__invocation_3_01S_00_01first__if__t_3_01void_00_01ifc__is__functor__ptr_3_01S_01_4_01_4_01_4.md)     
        * **struct** [**default\_invocation&lt; S, first\_if\_t&lt; void, ifc\_is\_member\_handle&lt; S &gt; &gt; &gt;**](structzmbt_1_1reflect_1_1default__invocation_3_01S_00_01first__if__t_3_01void_00_01ifc__is__member__handle_3_01S_01_4_01_4_01_4.md)     
        * **struct** [**has\_invocation\_for**](structzmbt_1_1reflect_1_1has__invocation__for.md) _Trait check for reflect::invocation&lt;T&gt;_ 
        * **struct** [**invocation**](structzmbt_1_1reflect_1_1invocation.md) _Interface reflection metafunction._ 
        * **struct** [**invocation&lt; S, detail::enable\_custom\_invocation&lt; S &gt; &gt;**](structzmbt_1_1reflect_1_1invocation_3_01S_00_01detail_1_1enable__custom__invocation_3_01S_01_4_01_4.md) 
        * **struct** [**invocation&lt; S, detail::enable\_default\_invocation&lt; S &gt; &gt;**](structzmbt_1_1reflect_1_1invocation_3_01S_00_01detail_1_1enable__default__invocation_3_01S_01_4_01_4.md) 
        * **struct** [**serialization**](structzmbt_1_1reflect_1_1serialization.md) _Serialization metafunction._ 
        * **struct** [**serialization&lt; T, detail::enable\_for\_custom\_serialization&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1serialization_3_01T_00_01detail_1_1enable__for__custom__serialization_3_01T_01_4_01_4.md) 
        * **struct** [**serialization&lt; T, detail::enable\_for\_default\_serialization&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1serialization_3_01T_00_01detail_1_1enable__for__default__serialization_3_01T_01_4_01_4.md) 
        * **struct** [**signal\_traits**](structzmbt_1_1reflect_1_1signal__traits.md) _Signal traits metafunction, providing default values._ 
        * **struct** [**signal\_traits&lt; T, detail::enable\_custom\_signal\_traits&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1signal__traits_3_01T_00_01detail_1_1enable__custom__signal__traits_3_01T_01_4_01_4.md) 
        * **struct** [**signal\_traits&lt; T, detail::enable\_default\_signal\_traits&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1signal__traits_3_01T_00_01detail_1_1enable__default__signal__traits_3_01T_01_4_01_4.md) 
        * **struct** [**signal\_traits&lt; T, detail::enable\_initialization\_trap&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1signal__traits_3_01T_00_01detail_1_1enable__initialization__trap_3_01T_01_4_01_4.md) _Unhandled types catcher._     
        * **struct** [**signal\_traits&lt; void &gt;**](structzmbt_1_1reflect_1_1signal__traits_3_01void_01_4.md)     
    * **struct** [**serialization\_error**](structzmbt_1_1serialization__error.md) _Failed serialization._     
    * **class** [**shared\_resource**](classzmbt_1_1shared__resource.md)     
    * **struct** [**test\_assertion\_failure**](structzmbt_1_1test__assertion__failure.md) _Failed test assertion._     
    * **struct** [**type\_tag**](structzmbt_1_1type__tag.md) _Type tag to pass template parameters as function arguments._     
* **struct** [**PersistentConfig**](structzmbt_1_1Config_1_1PersistentConfig.md)     
* **struct** [**Registry**](structzmbt_1_1OutputRecorder_1_1Registry.md)     
    * **struct** [**FrameBuffs**](structzmbt_1_1OutputRecorder_1_1Registry_1_1FrameBuffs.md)     
* **struct** [**internal\_ctor**](structzmbt_1_1Trigger_1_1internal__ctor.md) 
* **struct** [**Handle**](structzmbt_1_1lang_1_1Operator_1_1Handle.md)     
    * **struct** [**Handle**](structzmbt_1_1lang_1_1Operator_1_1Handle_1_1A.md)     
    * **struct** [**Handle**](structzmbt_1_1lang_1_1Operator_1_1Handle_1_1B.md)     
    * **struct** [**Handle**](structzmbt_1_1lang_1_1Operator_1_1Handle_1_1C.md)     
    * **struct** [**Handle**](structzmbt_1_1lang_1_1Operator_1_1Handle_1_1D.md)     
    * **struct** [**Handle**](structzmbt_1_1lang_1_1Operator_1_1Handle_1_1L.md)     
    * **struct** [**Handle**](structzmbt_1_1lang_1_1Operator_1_1Handle_1_1S.md)     
* **struct** [**T\_As**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__As.md) _T\_As transition._     
* **struct** [**T\_At**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__At.md) _T\_At transition._     
* **struct** [**T\_ContinuePipe**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__ContinuePipe.md) _T\_ContinuePipe transition._     
* **struct** [**T\_CustomParam**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__CustomParam.md) _T\_CustomParam transition._     
* **struct** [**T\_Description**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Description.md) _T\_Description transition._     
* **struct** [**T\_Filter**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Filter.md) _T\_Filter transition._     
* **struct** [**T\_OnTrigger**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__OnTrigger.md) _T\_OnTrigger transition._     
* **struct** [**T\_Param**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Param.md) _T\_Param transition._     
* **struct** [**T\_ParamRow**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__ParamRow.md) _T\_ParamRow transition._     
* **struct** [**T\_PipeId**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__PipeId.md) _T\_PipeId transition._     
* **struct** [**T\_PostRun**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__PostRun.md) _T\_PostRun transition._     
* **struct** [**T\_PreRun**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__PreRun.md) _T\_PreRun transition._     
* **struct** [**T\_Repeat**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Repeat.md) _T\_Repeat transition._     
* **struct** [**T\_Tag**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Tag.md) _T\_Tag transition._     
* **struct** [**T\_Take**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Take.md) _T\_Take transition._     
* **struct** [**T\_TerminatePipe**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__TerminatePipe.md) _T\_TerminatePipe transition._     
* **struct** [**T\_Test**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__Test.md) _T\_Test transition._     
* **struct** [**T\_TestRow**](structzmbt_1_1mapping_1_1ModelDefinition_1_1T__TestRow.md) _T\_TestRow transition._     
* **struct** [**custom\_serialization&lt; decor::precise&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1custom__serialization_3_01decor_1_1precise_3_01T_01_4_01_4.md)     
* **struct** [**custom\_serialization&lt; decor::underlying&lt; T &gt; &gt;**](structzmbt_1_1reflect_1_1custom__serialization_3_01decor_1_1underlying_3_01T_01_4_01_4.md)     


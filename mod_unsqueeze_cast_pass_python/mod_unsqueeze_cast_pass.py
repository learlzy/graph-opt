from ge.passes import PassStage, PatternFusionPass, pattern, register_fusion_pass
from ge.es.math import Cast, Mod
from ge.es.custom import ModUnsqueezeCast


@register_fusion_pass(name="ModUnsqueezeCastPass", stage=PassStage.AFTER_ORIGIN_GRAPH_OPTIMIZE)
class ModUnsqueezeCastPass(PatternFusionPass):
    @pattern
    def mod_cast(self, inputs):
        print("define ModUnsqueezeCastPass")
        return Cast(Mod(inputs[0], inputs[1]))

    def replacement(self, inputs):
        return ModUnsqueezeCast(inputs[0], inputs[1])
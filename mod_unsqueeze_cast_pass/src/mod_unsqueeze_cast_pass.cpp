#include "es_all_ops.h"      // 内置：Mod, Cast, Const...
#include "es_custom_ops.h"   // 自定义：ModUnsqueezeCast
#include "ge/fusion/pass/pattern_fusion_pass.h"

using namespace ge;
using namespace fusion;

class ModUnsqueezeCastPass : public PatternFusionPass {
 protected:
  // 1) 要匹配的拓扑
  std::vector<PatternUniqPtr> Patterns() override {
    std::cout << "Define pattern for AddCustomZeroPass" << std::endl;
    std::vector<PatternUniqPtr> patterns;
    auto gb = es::EsGraphBuilder("mod_cast_pattern");
    auto x = gb.CreateInput(0);
    auto y = gb.CreateInput(1);
    auto mod = es::Mod(x, y);      // 内置
    auto cast = es::Cast(mod, DT_FLOAT16);     // 内置；若还有 attr 按 IR 补
    auto graph = gb.BuildAndReset({cast});
    auto pattern = std::make_unique<Pattern>(std::move(*graph));
    patterns.emplace_back(std::move(pattern));
    return patterns;
  }

  // 2) 可选：额外过滤（dtype、shape、属性等）
  bool MeetRequirements(const std::unique_ptr<MatchResult> &match_result) override {
    return true;  // 拓扑对就融
  }

  // 3) 替换成你的融合算子
  GraphUniqPtr Replacement(const std::unique_ptr<MatchResult> &match_result) override {
    auto gb = es::EsGraphBuilder("replacement");
    auto x = gb.CreateInput(0);
    auto y = gb.CreateInput(1);
    auto fused = es::ModUnsqueezeCast(x, y);  // 自定义 ES API
    return gb.BuildAndReset({fused});
  }
};

// 注册到 GE，指定执行阶段
REG_FUSION_PASS(ModUnsqueezeCastPass)
    .Stage(CustomPassStage::kAfterOriginGraphOptimize);
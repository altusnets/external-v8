// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_INTERPRETER_BYTECODE_PEEPHOLE_OPTIMIZER_H_
#define V8_INTERPRETER_BYTECODE_PEEPHOLE_OPTIMIZER_H_

#include "src/interpreter/bytecode-pipeline.h"

namespace v8 {
namespace internal {
namespace interpreter {

class ConstantArrayBuilder;

// An optimization stage for performing peephole optimizations on
// generated bytecode. The optimizer may buffer one bytecode
// internally.
class BytecodePeepholeOptimizer final : public BytecodePipelineStage,
                                        public ZoneObject {
 public:
  BytecodePeepholeOptimizer(ConstantArrayBuilder* constant_array_builder,
                            BytecodePipelineStage* next_stage);

  void Write(BytecodeNode* node) override;
  size_t FlushForOffset() override;
  void FlushBasicBlock() override;

 private:
  BytecodeNode* Optimize(BytecodeNode* current);

  void UpdateCurrentBytecode(BytecodeNode* const current);
  bool CanElideCurrent(const BytecodeNode* const current) const;
  bool CanElideLast(const BytecodeNode* const current) const;

  void InvalidateLast();
  bool LastIsValid() const;
  void SetLast(const BytecodeNode* const node);

  bool LastBytecodePutsNameInAccumulator() const;

  Handle<Object> GetConstantForIndexOperand(const BytecodeNode* const node,
                                            int index) const;

  ConstantArrayBuilder* constant_array_builder_;
  BytecodePipelineStage* next_stage_;
  BytecodeNode last_;
  bool last_is_discardable_;

  DISALLOW_COPY_AND_ASSIGN(BytecodePeepholeOptimizer);
};

}  // namespace interpreter
}  // namespace internal
}  // namespace v8

#endif  // V8_INTERPRETER_BYTECODE_PEEPHOLE_OPTIMIZER_H_

using UnityEngine;

namespace UnityPhysXFlow
{
    /// <summary>
    /// PhysX Flow Emitter Component - spawns fluid particles at a given position.
    /// </summary>
    [AddComponentMenu("PhysX Flow/Flow Emitter")]
    public class FlowEmitter : MonoBehaviour
    {
        [Header("Emitter Parameters")]
        [Tooltip("Radius of the emitter sphere")]
        [Range(0.1f, 10f)]
        public float radius = 1.0f;

        [Tooltip("Density of emitted fluid")]
        [Range(0.1f, 10f)]
        public float density = 1.0f;

        [Tooltip("Auto-create emitter on Start")]
        public bool autoCreate = true;

        private int _emitterHandle = -1;

        private void Start()
        {
            if (autoCreate)
            {
                CreateEmitter();
            }
        }

        private void Update()
        {
            if (_emitterHandle >= 0)
            {
                // Sync position and parameters to native side
                UnityPhysXFlow.SetEmitterParams(_emitterHandle, transform.position, radius, density);
            }
        }

        public void CreateEmitter()
        {
            if (_emitterHandle >= 0) return; // Already created

            _emitterHandle = UnityPhysXFlow.CreateEmitter(transform.position, radius, density);
            if (_emitterHandle < 0)
            {
                Debug.LogError($"[FlowEmitter] Failed to create emitter at {transform.position}");
            }
            else
            {
                Debug.Log($"[FlowEmitter] Created emitter {_emitterHandle} at {transform.position}");
            }
        }

        public void DestroyEmitter()
        {
            if (_emitterHandle < 0) return;

            UnityPhysXFlow.DestroyEmitter(_emitterHandle);
            Debug.Log($"[FlowEmitter] Destroyed emitter {_emitterHandle}");
            _emitterHandle = -1;
        }

        private void OnDestroy()
        {
            DestroyEmitter();
        }

        private void OnDrawGizmos()
        {
            Gizmos.color = Color.cyan;
            Gizmos.DrawWireSphere(transform.position, radius);
        }

        private void OnDrawGizmosSelected()
        {
            Gizmos.color = new Color(0, 1, 1, 0.3f);
            Gizmos.DrawSphere(transform.position, radius);
        }
    }
}

import numpy as np

alphabet = ['A', 'T', 'G', 'C']

def IDSChannel(tx_seqs, pi, pd, ps):
    numSeqs = len(tx_seqs)
    rx_seqs = [''] * numSeqs
    alphabet = ['A', 'T', 'G', 'C']
    # Markov process
    for ind, tx in enumerate(tx_seqs):
        N = len(tx)
        rx = ''
        i = 0  # state number
        while i < N:
            # Insertion
            if randSel(pi):
                rx = rx + alphabet[np.random.randint(0, 4)]
            else:
                # Deletion
                if ~randSel(pd):
                    # Substitution
                    if randSel(ps):
                        tmp = alphabet.copy()
                        tmp.remove(tx[i])
                        rx = rx + tmp[np.random.randint(0, 3)]
                    else:
                        rx = rx + tx[i]
                i = i + 1
        rx_seqs[ind] = rx
    return rx_seqs

def randSel(p):
    sel = np.random.rand() < p
    return sel

if __name__ == '__main__':
    # Parameters
    pi = 0.015
    pd = 0.015
    ps = 0.015
    length = 110
    reads = 30000
    clusterSize = 10
    
    alphabet_array = np.array(alphabet)
    # Generate random DNA sequences as centers
    centers = [''] * clusterSize
    with open('../Data/generatedCenters.txt', 'w') as f:
        f.truncate(0)
        for i in range(clusterSize):
            center = alphabet_array[np.random.randint(0, 4, (length, ))]
            centers[i] = ''.join(center)
            f.write(''.join(center) + '\n')

    copies = [[] for _ in range(clusterSize)]
    for i in range(reads):
        picked = np.random.randint(0, clusterSize)
        pickedCenter = centers[picked]
        noisyCopy = IDSChannel(pickedCenter, pi, pd, ps)
        copies[picked].append(noisyCopy)

    with open('../Data/generatedCopies.txt', 'w') as f:
        f.truncate(0)
        for i in range(clusterSize):
            for copy in copies[i]:
                f.write(''.join(copy) + '\n')
            f.write('===========================================\n')

    